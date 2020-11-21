# <center>ESoft Crosshairs</center>
> Simple app to draw a crosshair in any game window

## Configuration
Configuration is now done using a JSON file (finally!)
So you no longer have to rebuild the application to modify the target window and crosshair design. Run the app once to generate the default `config.json` file to make things easier.

> Default `config.json`
```json
{
    "target": "FiveM", // Target Window Title
    "colour": 255,	   // Crosshair Colour (UINT)
    "thickness": 3,	   // Crosshair Thickness
    "width": 36,	   // Crosshair Width
    "height": 36	   // Crosshair Height
}
```

## Building
> This project was created with Visual Studio 2019, compatiblity with previous versions cannot be guaranteed.

#### 🔵 Clone the project from github:
```bash
git clone https://github.com/ElysiumSoft/es-crosshairs
```

#### 🔵 Open `ElysiumCrosshairs.vcxproj` in VS2019, it can be found in the root directory of the newly cloned repository.

#### 🔵 Edit the variables in the `IntializeGlobals` function to meet your requirements.

#### 🔵 Finally, build the project using Build->Build Solution or F5. And then run the built app

## Usage

#### Simply run the application and it will draw onto the window with the title specified in the `InitializeGlobals` function. To exit press <kbd>INSERT</kbd>

## Controls
<center>

| Key (Combination) | <center>Function</center> |
|:-----------------:|:---------|
| <kbd>F4</kbd> | Turn the crosshair on & off |
| <kbd>F5</kbd> | Reload values from config file (update crosshair without restarting app) |

</center>


