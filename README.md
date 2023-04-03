# Krint.c
Graphics library by Kirdow made in C

## Build & Run

First build the demo
```sh
./build.sh
```

Then run the demo
```sh
./run.sh
```

# Examples
The demo currently generates tests when run with --record=true,
these are then saved to [``./tests/``](./tests/) and added to this repository.
When executing the demo using ``./run.sh`` a directory called ``./run`` gets created,
and any generated files are created in there. These will at some point be matched at
runtime in the demo towards the files in ``./tests/`` to make sure the code works as
intended. For the most part, the images in ``./run/`` should match the ones in ``./tests/``.

Anyhow, here are a few examples from the [``./tests/``](./tests/) directory.

## japan.png
This generates a correct Japan flag

![image of japan flag test](./tests/japan.png "Japan Flag Test")

## lines.png
This generates lines in a circular-ish shape

![circular lines test](./tests/lines.png "Circular Lines Test")

# Credits
Inspired by [Olive.c](https://github.com/tsoding/olive.c) by [Tsoding](https://www.youtube.com/@TsodingDaily).