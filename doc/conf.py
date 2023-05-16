# -- Project information -----------------------------------------------------
from pathlib import Path

project = 'Erbsland Qt TOML'
copyright = '2023, Erbsland DEV'
author = 'Erbsland DEV'
release = '1.2'

# -- General configuration ---------------------------------------------------
extensions = ['sphinx_rtd_theme', 'breathe']
templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for HTML output -------------------------------------------------
html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

# -- Options for Breathe -----------------------------------------------------
_project_dir = Path(__file__).parent.parent
_src_dir = _project_dir / 'src'
breathe_projects = {"erbsland-qt-toml": str(_project_dir / '_build/breathe/doxygen/erbsland-qt-toml/xml')}
breathe_default_project = "erbsland-qt-toml"
breathe_projects_source = {
    "erbsland-qt-toml": (str(_src_dir), ['erbsland/qt', 'erbsland/qt/toml'])
}
breathe_doxygen_config_options = {
    'STRIP_FROM_PATH': str(_src_dir),
    'STRIP_FROM_INC_PATH': str(_src_dir),
}

