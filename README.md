![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/Robotics-Society-PEC/Hovercraft/test.yml?style=plastic)
![GitHub License](https://img.shields.io/github/license/Robotics-Society-PEC/Hovercraft?style=plastic)
![GitHub Repo stars](https://img.shields.io/github/stars/Robotics-Society-PEC/Hovercraft?style=plastic)


# Hovercraft

## Introduction

Welcome to our project repository! Here you'll find all the resources, documentation, and source code related to our project.

## About Hovercraft

This hovercraft was designed using 2 2400 emax 2207 motors , xylo 40A ESC and ESP32.

The Esp32 is responsible for decoding the sbus signal from the reciever and then produce the output signals for motor. The main reason of using ESP32 was so that we could reverse the direction of motor using 3d mode which is run over the dshot protocol. This nabled the hovercraft to go in the reverse direction if required even if that was with very less efficiency.

<!-- ## Features

- Feature 1: [Brief description]
- Feature 2: [Brief description]
- Feature 3: [Brief description] -->

## Getting Started

To get started, follow these steps:

1. Clone this repository:

    ```bash
    git clone https://github.com/Robotics-Society-PEC/Hovercraft.git
    ```

## Usage

[Explain how to use your project, provide code examples, etc.]

## Contributing

We welcome contributions from the community! If you'd like to contribute to [Your Project Name], please follow these guidelines:

- Fork the repository
- Create your feature branch: `git checkout -b feature-name`
- Commit your changes: `git commit -am 'Add some feature'`
- Push to the branch: `git push origin feature-name`
- Submit a pull request

For major changes, please open an issue first to discuss what you would like to change.

## License
See the [LICENSE](LICENSE) file for details.

