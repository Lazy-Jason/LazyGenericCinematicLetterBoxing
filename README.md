# Lazy Generic Cinematic Letterboxing Plugin

## Overview

The Lazy Generic Cinematic Letterboxing Plugin is a versatile tool for Unreal Engine projects that allows developers to easily add and manage cinematic letterboxing effects. This plugin provides smooth transitions, customizable aspect ratios, and runtime control, making it perfect for creating immersive cinematics and dynamic gameplay sequences.

## Features

- Smooth transitions between letterboxed and full-screen views
- Customizable target aspect ratios
- Runtime control for dynamic cinematics
- Easy integration with existing projects
- Configurable settings through the Project Settings menu

## Requirements

- Unreal Engine 5.1.0 or later

## Installation

1. Clone or download this repository.
2. Copy the plugin folder into your Unreal Engine project's `Plugins` directory.
3. Restart the Unreal Engine Editor if it's currently running.
4. Enable the plugin in your project settings.

## Usage

### Setting Up

1. In your project settings, navigate to the "Lazy Cinematic Letter Boxing" section.
2. Set the path to your letterbox material in the "Letter Box Material Path" field.
3. Adjust the "Default Target Aspect Ratio" and "Tick Rate" as needed.

### In Blueprints

To use the letterboxing effect in your game:

1. Get the Lazy Letter Boxing Subsystem:
   ```cpp
   GetGameInstance()->GetSubsystem<ULazyLetterBoxingSubsystem>()
   ```

2. To open the letterbox effect:
   ```cpp
   OpenCinematicLetterBoxing(float TransitionTime = 0.5f, float AspectRatio = 0.0f)
   ```

3. To close the letterbox effect:
   ```cpp
   CloseCinematicLetterBoxing(float TransitionTime = 0.5f)
   ```

4. To check if letterboxing is currently enabled:
   ```cpp
   IsLetterBoxingEnabled()
   ```

### Best Practices

- Use smooth transitions for a more cinematic feel.
- Experiment with different aspect ratios to find what works best for your scenes.
- Consider using letterboxing to draw focus to important elements in your game.

## Configuration

You can customize the plugin's behavior in the Project Settings:

- `Letter Box Material Path`: Set the path to your custom letterbox material.
- `Default Target Aspect Ratio`: Set the default aspect ratio for letterboxing (between 0 and 1).
- `Tick Rate`: Adjust the update frequency of the letterboxing effect.

## Support

If you encounter any issues or have questions, please file an issue on the GitHub repository.

## Acknowledgements

Special thanks to all contributors and users of this plugin.

---
## License

Copyright (C) 2024 Job Omondiale - All Rights Reserved.

This plugin is licensed under the BSD-3-Clause license found in the
LICENSE file in the root directory of this source tree.
