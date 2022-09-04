# `!Draw2SVG`

(This application also makes use of the desktop's interactive help facility.
Consult your computer's documentation.)

Start the application by double clicking on its icon in the directory that you installed it in.
The icon should appear on the icon bar.

Open a directory containing a draw file to be converted, and another to store the generated SVG file in.

Drag the draw file to the icon on the icon bar.
A window appears, showing the corresponding SVG file and a suggested file name &ndash; adjust it if you see fit.
Draw the SVG file to the window where you want to save it.
The conversion begins (sorry &ndash; this is not multitasking).
When it stops, you will see the new SVG file in the place that you dropped it.

*Beware!*
If you drag the SVG file to a directory already containing a file with the same name, the old file will be overwritten.
Watch out for truncated filenames too;
for example, if you have a draw file called `MyTestFile`, and drag it to the converter, the SVG file will have the suggested name `MyTestFile/svg` &ndash; if you save this to the same directory as the original file, and the filing system is limited to 10-character file names, the SVG file may overwrite the draw file, since their names are identical in the first 10 characters.


# Options

The conversion options can be brought up from the icon bar menu `Options...`.
Clicking `OK` applies any changes and closes the window.
Clicking `Save` applies and saves changes.
Clicking `Restore` restores previously saved changes.
Clicking `Cancel` aborts any changes.


## `Preserve position` &mdash; Positioned SVGs

Select `Preserve position` to add x and y attributes to the output SVG to get, for example:

    <svg x="0.5cm" y="3cm" width="...
  
rather than:

    <svg width="...

Position is usually determined by context, so `x` and `y` are normally ignored.


## `Preserve groups` &mdash; Group conversion

Select `Preserve groups` to enable conversion of Drawfile group objects into SVG `<g>` elements.
This should have no effect on rendering, but may be helpful if the SVG is subsequently to be edited.


## `Size of thin lines:` &mdash; Thin-line thickness

Adjust the field marked `Thinness` to how thick 'thin' lines in Drawfiles should appear as SVG lines.


## `Scale by:`, `Fit to:`, `Don't scale or fit` &mdash; Scaling

Select `Scale by:` and enter the factor in the adjacent field to produce an SVG scaled up from the original Drawfile by that factor.
`3,2` specifies a greater horizontal scaling than vertical.

Select `Fit to:` to produce and SVG scaled to fit a particular size.
Specify the width and height in the adjacent field:

* `3cm,4cm` produces a specific width and height.

* `3cm,*` scales to fit a specific width and preserves the aspect ratio.

* `*,4cm` scales to fit a specific height and preserves the aspect ratio.

Select `Don't scale or fit` to apply no scaling.


## `Absolute/Percent/None` &mdash; Include width and height attributes

Select `Absolute` to cause the `<svg>` element to include width and height values using preferred units.
Select `Percent` to express width and height as percentages.
Select `None` to omit width and height attributes.
Some applications may have difficulty processing an SVG without them, but they often have the disadvantage of preventing other applications from performing any scaling on the image at all, which rather defeats the purpose of a vector-graphic format.


## Aspect ratio

This scaling works by adjusting the output SVG width and height values (which are normally computed from the Drawfile's dimensions) according to the scaling or fitting parameters.  An SVG viewer has to map the co-ordinate system used within the generated SVG (the values without units, delimited by the viewBox attribute) onto the physical width and height specified at the start of the SVG, so adjusting these values can produce an overall scaling of the image, including a change in aspect ratio (when fitting to a specific width and height - `3cm,4cm` - or when scaling the dimensions separately - `3,2`).

However, by default, SVG viewers attempt to preserve the aspect ratio even if the viewBox size and the SVG width and height don't have the same aspect ratio.  The image will either be scaled down just enough to fit in, or up just enough to cover, the required area.  The former method is 'meet' (the default), and the latter is 'slice'.  There are further options to specify the position that a reduced image occupies within the area, or which part of an expanded image is displayed. It is also possible to turn the aspect-ratio preservation off.

See the SVG spec under `Coordinate Systems...` for further details.


## `Margin` &mdash; Margin size

Select `Margin` and specify the size in the adjacent field to add a transparent margin around the SVG.
For example, `3cm` specifies a margin the same size all around the image, whereas `3cm,2cm` specifies left/right and top/bottom margin sizes separately.
The margin is not subjected to scaling.


## `Background` &mdash; Background colour

Select `Background` and enter a colour specification in the adjacent field to add a solid background (which also covers the margin).

Colours take the form #rgb or `#rrggbb`, where `r`, `g` and `b` are hexadecimal digits `0-9`, `a-f`.


## Output units

An uneditable field displays the units used in the output: `pt`
(points), `mm` (millimetres), `cm` (centimetres), `in` (inches).  The
adjacent menu button allows the preferred option to be selected.


## `Autosave` Automatic saving

To perform multiple conversions quickly, select `Autosave` and drag a directory onto the adjacent field to indicate where you want SVGs to be saved.
(You can also drag a file within that directory to achieve the same effect.)

The results of any subsequent conversions you perform will be automatically saved into the directory using a name derived from the source file.

*Files with the same name will be overwritten!*


# Trouble-shooting

* If this error occurs when you run the application:

  ```
  Can't find the draw2svg program in <Run$Path>
  ```

  &hellip;you need the executable file `draw2svg` from the zip archive `draw2svg/zip` to be placed in one of the directories listed when you type:

  ```
  *Show Run$Path
  ```

  &hellip;in a command window.
  The normal place to put it is `$.!Boot.Library` on your hard disc.

* If this error occurs when you drop the SVG file into its position:

  ```
  Possibly short of memory...?
  ```

  &hellip;increase the `WimpSlot` values in the file `!Run`.
  This shouldn't really happen.
