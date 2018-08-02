# mesobot-calib
Calibration process for the Mesobot. Written using OpenCV 3.4.1

## Usage
To use the application, run its executable from the command line.

Example:
```bash
./stereo_calib -l calib/left -r calib/right -w 9 -h 6 -length 0.0225 -thresh 1.4 -check
```

### Output
The calibration information and settings will be placed into `calib_settings.yml`. Maps for OpenCV's remap function will be placed into `undistort_maps.yml`.

### Options
There are a variety of options available for the calibration process. Of them, only `-l`, `-r`, `-w`, `-h`, `-length`, and `-thresh` are required for the program to function.

#### Required
`-l` Specifies the folder containing the left images.

`-r` Specifies the folder containing the right images.

`-w` Specifies the width of the chessboard pattern (inner corners).

`-h` Specifies the height of the chessboard pattern (inner corners).

`-length` Specifies the side edge of a square on the chessboard pattern (in meters).

`-target` Specifies the target overall error of the calibration. If left blank, program will target and error of 0.

`-thresh` Specifies the threshold value scalar. This value will be multiplied by the average overall reprojection error to eliminate image pairs with a maximum individual (either image's) error above the scaled threshold.

#### Solution methods
`-lu` Enables Gaussian elimination solution method (very fast).

`-qr` Enables QR decomposition solution method (faster than normal).

#### Stereo calibration models
*These models can be used alone or in conjunction with each other. They specify which distortion coefficients will be enabled in the stereo calibration process.*

`-rational` Enables the rational model, adding the radial distortion coefficients k4, k5, and k6.

`-thinprism` Enables the thin prism model, adding the thin prism distortion coefficients s1, s2, s3, and s4.

`-tilted` Enables the tilted model, adding the tilt distortion coefficients tauX and tauY.

#### Stereo calibration parameters [[more info]](https://docs.opencv.org/3.4.1/d9/d0c/group__calib3d.html#ga91018d80e2a93ade37539f01e6f07de5)
`-k1` to `-k6` Fixes the corresponding radial distortion coefficient during calibration.

`-s` Fixes the thin prism disortion coefficients (use only with `-thinprism`).

`-tau` Fixes the tilt distortion coefficients (use only with `-tilted`).

`-fixar` Fixes the aspect ratio.

`-fixp` Fixes the principal points during the optimization.

`-fixtan` Fixes the tangential disortion coefficients.

`-zerotan` Fixes the tangential distortion coefficients to 0.

`-fixf` Fixes the focal lengths.

`-samef` Enforces equal focal lengths for the cameras.

#### Chessboard pattern detection parameters [[more info]](https://docs.opencv.org/3.4.1/d9/d0c/group__calib3d.html#ga93efa9b0aa890de240ca32b11253dd4a)
`-at` Enables adaptive thresholding.

`-ni` Enables image normalization.

`-fq` Enables filtering out of false quads extracted at the contour retrieval stage.

`-check` Checks images quickly for a chessboard pattern before intensive search. Can save time during chessboard detection.
