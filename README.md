# DSA Visualization (CS163)

A graphical Data Structures and Algorithms (DSA) visualization tool built in C++ using the **raylib** library. This interactive application is designed to help users understand how various data structures work under the hood through step-by-step visual feedback and animations.

## 🚀 Features

The application includes an interactive main menu that allows you to navigate between different data structure visualizations:

* **Linked Lists:** Visualize node creation, insertion, deletion, and traversal.
* **Trees:** Includes visualizations for various tree types (AVL Tree, 2-3-4 Tree, Trie, and Max Heap).
* **Hash Tables:** Visualize hashing mechanisms, collision resolution, and data mapping.
* **Graphs:** Interactive graph traversal and pathfinding.
* **Animation Controls:** Play, pause, reset, and step through the algorithm animations to easily track changes in the data structure.

## 🛠️ Technologies Used

* **Language:** C++ (C++14 Standard)
* **Graphics & UI Library:** [raylib](https://www.raylib.com/) 
* **Build System:** Make (Cross-platform Makefile included)

## 📂 Project Structure

* `main.cpp`: The main entry point of the application handling the core application loop, window initialization, and screen state management.
* `header/`: Contains all the C++ header files (`.h`) declaring the visualizer classes (e.g., `LinkedListVisualizer.h`, `treevisual.h`, `graphvisual.h`).
* `source/`: Contains the C++ implementation files (`.cpp`) for the core application logic and data structures.
* `asset/`: UI elements, icons, and fonts used within the graphical application (e.g., play/pause buttons, structure logos).
* `Sample/`: Sample text files (e.g., `Graph.txt`, `HashTable.txt`) for loading pre-configured data.

## ⚙️ Prerequisites

To compile and run this project, you will need the following installed on your system:

1.  **C++ Compiler:** GCC (MinGW for Windows) or Clang supporting C++14.
2.  **Make:** For running the provided Makefile.
3.  **raylib:** The project relies on raylib. The Makefile is configured to look for the library binaries and headers (defaults to paths like `C:/raylib/` on Windows or `/usr/local/lib` on Linux/macOS). You may need to adjust the `RAYLIB_PATH` or `COMPILER_PATH` in the `Makefile` depending on your specific installation directory.

## 💻 Building and Running

### On Windows / Linux / macOS:

1. Clone or download the repository.
2. Open a terminal or command prompt in the root directory of the project (where the `Makefile` is located).
3. Build the project by running:
```bash
make
```

4. Once successfully compiled, run the executable:
    * **Windows:** `./game.exe`
    * **Linux/macOS:** `./game`

### Cleaning the build

To clean up compiled object files and the executable, run:

```bash
make clean
```

## 🎮 How to Use

1. Launch the application. You will be greeted by the **Main Menu**.
2. Click on the data structure you wish to explore (Linked List, Hash Table, Tree, or Graph).
3. Use the on-screen input fields and buttons to insert, delete, or search for data.
4. Use the playback controls (Play, Pause, Replay, Step Forward/Backward) to observe the algorithm in action.
5. Click the "Back" or "Main Menu" buttons to return to the selection screen.
