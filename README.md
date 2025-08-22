# BinaryRealm

**BinaryRealm** is a dynamic, Windows-only desktop environment featuring custom themes, animated visuals, widget-driven customization, and integrated console with custom commands and media embedding.

---

## Features
- **Themes system** – Customizable themes for the whole environment. 
- **Multi Monitor Support** – Supports multiple monitors. 
- **Exportable Settings** – Saved to `%AppData%\BinaryRealm` for easy portability.  
- **Configurable UI** – Built-in interface to adjust the environment.  
- **Animated Background & Windows** – Enhance aesthetics with motion and style.  
- **Built-in Console** – Access command-line directly in your desktop.  
- **Widget System + Taskbar** – Add, remove, and reorder widgets as desired.  
- **Media Embedding Area** – Place photos right on your desktop with animated changes.  

---

## Examples
<img width="1919" height="1079" alt="image" src="https://github.com/user-attachments/assets/777147a4-aa13-479c-9684-a1804ec09aee" />
<img width="1919" height="1078" alt="image" src="https://github.com/user-attachments/assets/99c17207-d6b4-4335-af19-de4ade96ce90" />
[Watch the demo video]([demo.mp4](https://github.com/user-attachments/assets/eb49d87e-08b5-4e52-87e5-c7a6a32fec06))

## Installation

### Prerequisites

- Windows 10 or later  
- C++20 compiler (MSVC recommended)  
- [CMake](https://cmake.org/) and [Ninja](https://ninja-build.org/) installed  
- Visual Studio 2019/2022

### Build & Run

Use the provided scripts in the `scripts` folder for setup:

- **`generate-cmake-ninja.bat`** – runs CMake using Ninja and builds the project.  
- **`generate-cmake-vs2022.bat`** – generates a Visual Studio solution to open and build directly in Visual Studio.  

```bash
git clone https://github.com/Camellia-ESL/BinaryRealm.git
cd BinaryRealm\scripts

# Option 1: Generate a cmake Ninja build
generate-cmake-ninja.bat

# Option 2: Generate a Visual Studio 2022 solution
generate-cmake-vs2022.bat

# Build: Join build directory and build.
```

### First Launch

- On first run, BinaryRealm will create a config directory at %AppData%\BinaryRealm.

- Use the built-in settings UI to customize widgets, animated backgrounds, console, and media embedding. **You can find and customize the keybinds needed to open the customization window or any other window in keybinds.h**
