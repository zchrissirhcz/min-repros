# test_spng_png16

## Intro
This directory serves as the minimal reproduce example for "saving 16 bit grayscale png image with libpng and libspng, compare their performance".

The expected result is libspng's time cost is less than libpng, or nealy same.

The actual result is, libspng is 6~8 times slower than libpng.

## Environment
- OS: 1) Linux x64  2) Android ARM64
- Dependencies
    - libspng: The latest source. With Optimization enabled.
    - libpng: [1.6.38](https://github.com/glennrp/libpng) for performance compare.
    - OpenCV: [4.5.5](https://github.com/opencv/opencv/releases/tag/4.5.5), for utility / helper.


## Time cost compare
### Linux-x64 output
```bash
cd build
./linux-x64-build.sh
./linux-x64-run.sh
```

```
(base) zz@home% ./linux-x64-run.sh 
  libpng: took   2.122 ms
    spng: took  13.385 ms
```

### Android ARM64 output
```bash
cd build
./android-arm64-build.sh
./android-arm64-run.sh
```

```
(base) zz@home% ./android-arm64-build.sh 
  libpng: took   2.189 ms
    spng: took  18.013 ms
```

### snapshot
![](libpng_spng_timecost_compare.png)