<div align="center">
  <img src="https://github.com/Robotics-Society-PEC/Hovercraft/actions/workflows/build.yml/badge.svg" alt="PlatformIO CI">
  <img src="https://img.shields.io/github/v/release/Robotics-Society-PEC/Hovercraft" alt="GitHub Release">
  <img src="https://img.shields.io/github/license/Robotics-Society-PEC/Hovercraft" alt="GitHub License">
  <img src="https://img.shields.io/github/stars/Robotics-Society-PEC/Hovercraft?style=flat" alt="GitHub Repo stars">
  <a href="https://gitpod.io/#https://github.com/Robotics-Society-PEC/Hovercraft.git">
    <img src="https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod" alt="Gitpod Ready-to-Code">
  </a>
</div>

# Hovercraft
Welcome to our project repository! Here you'll find all the resources, documentation, and source code related to our project.

## About Hovercraft

This hovercraft was designed using 2 2400 emax 2207 motors , xylo 40A ESC and ESP32.

The Esp32 is responsible for decoding the sbus signal from the reciever and then produce the output signals for motor. The main reason of using ESP32 was so that we could reverse the direction of motor using 3d mode which is run over the dshot protocol. This nabled the hovercraft to go in the reverse direction if required even if that was with very less efficiency.

## Features
- Multiple rc protocol support including crsf and sbus.
- supports dshot output and motor reversibility .

with many more features on the way!

## Getting Started

### Setting up Development Enviorment
To set up your development environment, follow these steps:

1. Install Visual Studio Code (VS Code):
- Download and install VS Code from the [official website](https://code.visualstudio.com/).

2. Install PlatformIO Extension:
- Open VS Code.
- Go to the Extensions view by clicking on the square icon in the sidebar or pressing Ctrl+Shift+X.
- Search for "PlatformIO" in the Extensions Marketplace.
- Click on "Install" to install the PlatformIO extension.
- Restart VS Code after installation is complete.


### Cloning the Repository
To get started with the project, follow these steps:

1. Clone the repository to your local machine:

    ```bash
    git clone https://github.com/Robotics-Society-PEC/Hovercraft.git
    ```
2. Open the project in Visual Studio Code (VS Code):

- Launch VS Code.
- Click on "File" > "Open Folder...".
- Navigate to the directory where you cloned the repository (Hovercraft) and select it.
- The project will open in VS Code, and you can start working on it.

## Contributing

We welcome contributions from the community! If you'd like to contribute, please follow these guidelines:

1. Fork the repository
2. Create your feature branch: 
    ```bash
    git checkout -b feature-name
    ```
3. Commit your changes:
    ```bash
    git commit -am 'Add some feature'
    ```

4. Push to the branch: 
    ``` bash
    git push origin feature-name
    ```
5. Submit a pull request

For major changes, please open an issue first to discuss what you would like to change.


## License
See the [LICENSE](LICENSE) file for details.