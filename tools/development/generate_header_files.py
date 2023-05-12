# Copyright © 2023 Tobias Erbsland. Web: https://erbsland.dev/
# Copyright © 2023 EducateIT GmbH. Web: https://educateit.ch/
#
# This program is free software: you can redistribute it and/or modify it under the terms of the
# GNU Lesser General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License along with this program.
# If not, see <https://www.gnu.org/licenses/>.


import argparse
import re
import shutil
import tomllib
from datetime import date
from pathlib import Path


class ScriptError (Exception):
    """
    Script error used for error reporting.
    """
    pass


class WorkingSet:
    """
    The working set for this script.
    """

    RE_FWD_ENTRIES = re.compile(R"""
        //\s*fwd-entry:\s*(.+)
    """, re.VERBOSE)

    def __init__(self):
        """
        Create a new empty working set.
        """
        self.verbose = False                       # If verbose messages shall be shown
        self.project_dir = Path()                  # The project directory
        self.header = ''                           # The header text.
        self.exclude_dirs: list[str] = []          # Parent directories to include.
        self.create_global_includes: bool = False  # If includes of submodules shall also copy into global space.
        self.create_all_base_dir = ''              # The base dir from where `all.hpp` header shall be created.
        self.src_dir = Path()                      # The directory with the sources.
        self.include_dir = Path()                  # The directory with the includes.
        self.dir_map: dict[str, list[str]] = {}    # A map with all project subdirectories.
        self.file_map: dict[str, Path] = {}        # A map with all header file names and their paths
        self.fwd_entry_map: dict[str, list[str]] = {}  # A map with all collected fwd entries.

    def write_all_header(self, header_dir, header_files):
        """
        Write a single 'in source' header file.

        :param header_dir: The directory of the header file.
        :param header_files: A list of header files to include.
        """
        header_path = self.src_dir / header_dir / 'all.hpp'
        if self.verbose:
            print(f'- writing: {header_dir}/app.hpp')
        text = self.header.replace('{year}', str(date.today().year))
        for header_file in header_files:
            text += f'#include "{header_file}"\n'
        text += '\n\n'
        header_path.write_text(text, encoding='utf-8')

    def write_include_header(self, target_path: Path, source_path: Path = None):
        """
        Write an include header that points to a header in the `src` directory.

        :param target_path: The path to the header file in the `src` directory.
        :param source_path: The optional relative path in the `include` directory.
        """
        if not target_path.is_file():
            raise ScriptError(f'Header file {target_path} not found.')
        if not source_path:
            source_path = Path(target_path.name)
        source_path = self.include_dir / source_path
        # get the number of directories to the project dir
        up_count = len(source_path.relative_to(self.project_dir).parts) - 1
        rel_target_path = Path(('../' * up_count) + 'src/erbsland') / target_path.relative_to(self.src_dir)
        if self.verbose:
            print(f'  writing "{source_path}" -> "{rel_target_path}"')
        text = f'#include "{rel_target_path}"\n'
        if not source_path.parent.is_dir():
            source_path.parent.mkdir(parents=True, exist_ok=True)
        source_path.write_text(text, encoding='utf-8')

    def collect_header_files(self):
        """
        Scan all subdirectories and generate leaf to root.
        """
        if self.verbose:
            print('Scanning source dir')
        for path in self.src_dir.rglob('*.hpp'):
            if path.name == 'all.hpp':
                if self.verbose:
                    print(f'  Skipping "all.hpp": {path.relative_to(self.src_dir)}')
                continue   # Skip `all.hpp` headers, to avoid recursion loops.
            if path.parts[-2] in self.exclude_dirs:
                if self.verbose:
                    print(f'  Skipping excluded: {path.relative_to(self.src_dir)}')
                continue
            rel_path = path.relative_to(self.src_dir)
            if len(rel_path.parts) >= 2:
                header_dir = rel_path.parent.as_posix()
                if not str(header_dir) in self.dir_map:
                    self.dir_map[str(header_dir)] = []
                header_file_name = rel_path.name
                if header_file_name != 'fwd.hpp':
                    self.dir_map[str(header_dir)].append(header_file_name)
                if self.verbose:
                    print(f'Adding header "{header_file_name}" from dir "{header_dir}"')
                if path.name == 'fwd.hpp':
                    header_file_name = f'fwd_{str(header_dir).replace("/", "_")}.hpp'
                if header_file_name in self.file_map:
                    raise ScriptError(f'Header file name conflict with `{header_file_name}`!')
                self.file_map[header_file_name] = path

    def collect_fwd_entries(self):
        """
        Search for relevant public class/enum declarations to put into `fwd.hpp` files.
        """
        if self.verbose:
            print('Scanning for fwd entries')
        for header_path in self.file_map.values():
            header_dir = str(header_path.parent.relative_to(self.src_dir))
            text = header_path.read_text(encoding='utf-8')
            entries = self.RE_FWD_ENTRIES.findall(text)
            if entries:
                if header_dir not in self.fwd_entry_map:
                    self.fwd_entry_map[header_dir] = []
                self.fwd_entry_map[header_dir].extend(entries)
                if self.verbose:
                    for entry in entries:
                        print(f'  in dir "{header_dir}" adding entry: {entry}')

    def generate_fwd_files(self):
        """
        Generate all `fwd.hpp` files.
        """
        if self.verbose:
            print('Generating `fwd.hpp` files')
        for header_dir, entries in self.fwd_entry_map.items():
            text = self.header.replace('{year}', str(date.today().year))
            text += '#include <cstdint>\n#include <type_traits>\n\n\n'
            text += f'namespace erbsland::{header_dir.replace("/", "::")} {{\n\n\n'
            text += ';\n'.join(entries) + ';\n\n\n'
            text += '}\n\n\n'
            path = self.src_dir / header_dir / 'fwd.hpp'
            if self.verbose:
                print(f'  writing: {path}')
            path.write_text(text, encoding='utf-8')

    def _create_all_header_for_dir(self, header_dir: str):
        # Only create `all.hpp` in subdirectories of `create_all_base_dir`
        if self.create_all_base_dir:
            if not header_dir.startswith(self.create_all_base_dir + '/'):
                return False
        else:
            if not header_dir:
                return False
        return True

    def _create_all_path_for_base_dir(self, header_dir: str) -> Path:
        if self.create_all_base_dir:
            header_dir = header_dir.replace(self.create_all_base_dir + '/', '')
            return Path(f'{self.create_all_base_dir}/all_{header_dir.replace("/", "_")}.hpp')
        return Path(f'all_{header_dir.replace("/", "_")}.hpp')

    def generate_all_headers(self):
        """
        Write the `all.hpp` files.
        """
        if self.verbose:
            print('Writing the "all.hpp" files to the `src` directory.')
        for header_dir, header_files in self.dir_map.items():
            if not self._create_all_header_for_dir(header_dir):
                continue
            header_files.sort()
            self.write_all_header(header_dir, header_files)

    def generate_includes(self):
        """
        Write all files in the `include` directory.
        """
        if self.verbose:
            print('Writing the individual header files in the `include` directory.')
        for header_dir in sorted(self.dir_map.keys()):
            if not self._create_all_header_for_dir(header_dir):
                continue
            target_path = self.src_dir / header_dir / 'all.hpp'
            self.write_include_header(target_path, self._create_all_path_for_base_dir(header_dir))
            self.write_include_header(target_path, Path(f'{header_dir}/all.hpp'))
        for header_file, path in self.file_map.items():
            self.write_include_header(path, path.relative_to(self.src_dir))
            if self.create_global_includes:
                self.write_include_header(path, Path(header_file))

    def remove_existing_includes(self):
        """
        Remove all existing include files to build them from scratch.
        """
        if self.include_dir.is_dir():
            shutil.rmtree(self.include_dir)
        self.include_dir.mkdir(parents=True, exist_ok=True)

    def read_config(self):
        print('Read the configuration')
        config_file = Path(__file__).with_suffix('.toml')
        if not config_file.is_file():
            raise ScriptError(f'Could not find configuration: {config_file}')
        config = tomllib.loads(config_file.read_text(encoding='utf-8'))
        self.header = config['header']
        self.exclude_dirs = config['excluded_directories']
        self.create_global_includes = config['create_global_includes']
        self.create_all_base_dir = config['create_all_base_dir']

    def parse_cmd(self):
        """
        Parse the command line arguments.
        """
        parser = argparse.ArgumentParser(
            description='Generate all includes, `fwd.hpp` and `all.hpp` header files.')
        parser.add_argument('-v', '--verbose', action='store_true')
        args = parser.parse_args()
        if args.verbose:
            self.verbose = True
        # No arguments for this script.
        self.project_dir = Path(__file__).parent.parent.parent
        self.src_dir = self.project_dir / 'src'
        if not self.src_dir.is_dir():
            raise ScriptError('The source dir does not exist.')
        self.src_dir = self.src_dir / 'erbsland'
        self.include_dir = self.project_dir / 'include'
        if not self.include_dir.is_dir():
            raise ScriptError('The include dir does not exist.')
        self.include_dir = self.include_dir / 'erbsland'

    def run(self):
        """
        Run this script.
        """
        self.parse_cmd()
        self.read_config()
        self.collect_header_files()
        self.generate_all_headers()
        self.remove_existing_includes()
        self.generate_includes()
        self.collect_fwd_entries()
        self.generate_fwd_files()


def main():
    try:
        ws = WorkingSet()
        ws.run()
    except ScriptError as err:
        exit(err)


if __name__ == '__main__':
    main()
