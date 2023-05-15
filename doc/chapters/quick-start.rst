
====================
Quick Start Tutorial
====================

This tutorial will guide you through the process of creating a simple application that uses the Erbsland Qt TOML library to read and display a TOML configuration file. This application will serve as an example to demonstrate how to integrate this library into a Qt application using CMake.

I you do not like to follow this tutorial, you will find this example application `here <https://github.com/erbsland-dev/erbsland-qt-toml-example/>`_.

Prerequisites
=============

Before we start, make sure you have the following installed:

- `CMake` version 3.25 or newer
- `Qt5` or `Qt6`
- `Git`

Setting up the Project Directory Structure
==========================================

We will first create the project directory structure.

1. Start by creating a new directory for the project. Let's call it `project`.

.. code-block:: bash

   mkdir project

2. Move into the `project` directory.

.. code-block:: bash

   cd project

3. Create a git submodule for the Erbsland Qt TOML library.

.. code-block:: bash

   git submodule add https://github.com/erbsland-dev/erbsland-qt-toml.git

4. Create a directory for the example application.

.. code-block:: bash

   mkdir example-app

5. Inside `example-app`, create `src` and `data` directories.

.. code-block:: bash

   mkdir example-app/src
   mkdir example-app/data

At this point, your directory structure should look like this:

.. code-block:: text

   project
   ├── erbsland-qt-toml
   └── example-app
       ├── data
       └── src

Creating the Application Source Files
=====================================

Now that we have our directory structure ready, let's create the required source files.

1. Create a new file `main.cpp` inside the `example-app/src` directory. Copy and paste the following content into `main.cpp`.

.. code-block:: cpp

   #include <erbsland/qt/toml/Parser.hpp>
   #include <erbsland/qt/toml/Error.hpp>

   #include <QCoreApplication>

   #include <iostream>

   using namespace elqt::toml;

   void displayValue(const ValuePtr &value, const QString &path) {
       std::cout << path.toStdString() << "\n";
       if (value->isTable()) {
           std::cout << "Table: " << value->size() << " Elements" << "\n\n";
           for (const auto &[key, tableValue] : value->toTable()) {
               displayValue(tableValue, path + '/' + key);
           }
       } else if (value->isArray()) {
           std::cout << "Array: " << value->size() << " Elements" << "\n\n";
           auto array = value->toArray();
           for (std::size_t i = 0; i < array.size(); ++i) {
               displayValue(array.at(i), path + QStringLiteral("/[%1]").arg(i));
           }
       } else {
           std::cout << "Value: " << value->toVariant().toString().toStdString() << "\n\n";
       }
   }

   int main(int argc, char *argv[]) {
       QCoreApplication application(argc, argv);
       ValuePtr toml{};
       try {
           auto path = QCoreApplication::applicationDirPath() + "/config.toml";
           Parser parser{};
           std::cout << "Reading configuration from: " << path.toStdString() << "\n";
           toml = parser.parseFile(path);
       } catch (const Error &err) {
           std::cerr << err.toString().toStdString() << std::endl;
          return 1;
       }
       displayValue(toml, "[root]");
       return 0;
   }

2. Now, create a TOML configuration file `config.toml` in the `example-app/data` directory. Copy and paste the following content into `config.toml`.

.. code-block:: toml

   #
   # Example Configuration
   #

   name = "Example"

   [main]
   title = "This is an Example"
   last_update = 2023-05-13 12:00:00

   [connection]
   services = [ "http", "ftp" ]
   ssl = true
   ports = [ 8080, 8081, 8022 ]

   [service.http]
   ip = "127.0.0.1"
   base_dir = "~/public_http"

   [service.ftp]
   ip = "127.0.0.1"
   base_dir = "~/public_http"

Setting up CMake
================

We will use CMake as our build system. Let's create two `CMakeLists.txt` files - one for the example application, and one for the project root.

1. In the `example-app` directory, create a file named `CMakeLists.txt`. Copy and paste the following content into `example-app/CMakeLists.txt`.

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.25)

   project(example-app)

   add_executable(example-app src/main.cpp)

   target_link_libraries(example-app PRIVATE erbsland-qt-toml)
   set_property(TARGET example-app PROPERTY CXX_STANDARD 17)
   target_compile_features(example-app PRIVATE cxx_std_17)
   add_custom_command(TARGET example-app POST_BUILD
           COMMAND ${CMAKE_COMMAND} -E copy
           "${CMAKE_CURRENT_LIST_DIR}/data/config.toml"
           "$<TARGET_FILE_DIR:example-app>/config.toml")

2. In the root of the `project` directory, create a file named `CMakeLists.txt`. Copy and paste the following content into `CMakeLists.txt`.

.. code-block:: cmake

   cmake_minimum_required(VERSION 3.25)

   project(erbsland-qt-toml-example)

   # Detect and load either Qt5 or Qt6.
   if (DEFINED Qt6_DIR AND NOT Qt6_DIR MATCHES "-NOTFOUND")
       find_package(Qt6 REQUIRED GLOBAL COMPONENTS Core)
   elseif (DEFINED Qt5_DIR AND NOT Qt5_DIR MATCHES "-NOTFOUND")
       find_package(Qt5 REQUIRED GLOBAL COMPONENTS Core)
   else()
       find_package(Qt6 GLOBAL COMPONENTS Core)
       if (NOT DEFINED Qt6_FOUND)
           find_package(Qt5 GLOBAL REQUIRED COMPONENTS Core)
       endif ()
   endif ()

   add_subdirectory(erbsland-qt-toml)
   add_subdirectory(example-app)

Building and Running the Application
====================================

With everything in place, it's time to build and run the application.

1. Move to the root of the `project` directory.

2. Create a new directory called `build`.

.. code-block:: bash

   mkdir build

3. Move into the `build` directory.

.. code-block:: bash

   cd build

4. Run `cmake` to configure the project. Make sure to replace `/path/to/qt` with the correct path to your Qt installation.

.. code-block:: bash

   cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt

5. Once configuration is done without errors, run `make` to build the application.

.. code-block:: bash

   make

6. Finally, run the application.

.. code-block:: bash

   ./example-app

That concludes the tutorial. By now, you should have built and run the Erbsland Qt TOML example application. When executed, the application reads the `config.toml` file and prints the configuration values to the standard output.

Remember that this is just a basic example. The Erbsland Qt TOML library offers much more functionality that you can use to handle TOML files in your Qt applications.

If you wish to expand on this, consider the following:

- Experiment with different TOML files and observe the output.
- Modify the `main.cpp` file to add more functionality, like writing to TOML files.
- Check out the documentation of the Erbsland Qt TOML library to learn more about its capabilities and how you can use them in your projects.
