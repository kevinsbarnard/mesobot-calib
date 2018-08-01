# mesobot-calib
Calibration process and example tracking for the Mesobot. Written using OpenCV 3.4.1

## Usage
To use the application, run its executable from the command line.

Example:
```bash
./stereo_calib -l calib/left -r calib/right -w 9 -h 6 -length 0.0225 -thresh 1.4 -check
```

### Options
There are a variety of options available for the calibration process. Of them, only `-l`, `-r`, `-w`, `-h`, `-length`, and `-thresh` are required for the program to function.

`-l` Specifies the folder containing the left images.

`-r` Specifies the folder containing the right images.

`-w` Specifies the width of the chessboard pattern (inner corners).

`-h` Specifies the height of the chessboard pattern (inner corners).

`length` Specifies the side edge of a square on the chessboard pattern (in meters).

`-target` Specifies the target overall error of the calibration.

`-thresh` Specifies the threshold value scalar. This value will be multiplied by the average overall reprojection error to eliminate image pairs with a maximum individual (either image's) error above the scaled threshold.

`-lu` Enables Gaussian elimination solution method (very fast).

`-qr` Enables QR decomposition solution method (faster than normal).

`-k1` to `-k6` Fixes the corresponding radial distortion coefficient during calibration.

`-at` Enables adaptive thresholding during chessboard detection.

`-ni` Enables image normalization during chessboard detection.

`-check` Checks images quickly for a chessboard pattern before intensive search. Can save time during chessboard detection.
