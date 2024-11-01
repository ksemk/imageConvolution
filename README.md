# Image Filter with Performance Measurement

This project demonstrates a custom image filtering application in C. The code loads a grayscale PNG image, applies a convolutional filter, measures the filtering performance, and outputs the filtered image. Key metrics such as memory bandwidth and pixel rendering speed are displayed in real-time. The project leverages cycle counting, low-level memory manipulation, and the `libpng` library for image handling.

## Features

- **Convolutional Image Filtering**: Applies a 3x3 kernel to enhance edges in grayscale images.
- **Performance Metrics**: Measures execution time, memory throughput, and rendering speed for benchmarking.
- **File I/O with PNG**: Utilizes `libpng` to read and write grayscale PNG files.
- **Cycle-Based Timing**: Uses processor cycle counting to calculate filter execution time.

## Requirements

- **C Compiler** (e.g., `gcc`)
- **libpng**: PNG library for C (`sudo apt-get install libpng-dev` on Debian-based systems)
- **GNU Make**: For compiling the project

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/ksemk/imageConvolution
   cd your-repo-name
   ```

2. **Build the Project**:
   Compile the program by running:
   ```bash
   make
   ```
   The `makefile` is configured to handle the compilation process automatically.

## Usage

Run the executable with a grayscale PNG file as input:

```bash
./program input.png
```

This command performs the following steps:
- Loads the input image
- Applies the convolutional filter to enhance edges
- Measures performance metrics such as execution time, memory throughput, and rendering speed
- Outputs the processed image as `out.png`

### Example Command

```bash
./program sample.png
```

### Output

- **Console Output**: Displays performance data including cycle count, execution time, memory bandwidth, and rendering speed.
- **Filtered Image**: The resulting image is saved as `out.png` in the current directory.

## Performance Measurement

The `measure_performance` function calculates metrics based on CPU cycles to assess filtering performance:
- **Cycles**: Number of CPU cycles used for filtering
- **Execution Time**: Time taken to apply the filter, calculated from cycles and CPU frequency
- **Memory Bandwidth**: Average data throughput in GB/s
- **Rendering Speed**: Pixel processing speed in Mpx/s

## Code Overview

- **filter**: Applies a convolutional kernel to each pixel to highlight image edges.
- **measure_performance**: Measures performance metrics.
- **main**: Manages file I/O, initializes `libpng`, and coordinates the filtering and performance measurement functions.

## Dependencies

This project uses the following files:
- `program`: Main executable file
- `makefile`: Automates compilation
- `timestamp64.s`: Assembly code to count CPU cycles
- `LICENSE`: License file for open-source usage

## Contacts

For any inquiries, support requests, or if you're interested in collaborating, please don't hesitate to reach out:

- **Email**: [kyrylo.semenchenko@gmail.com](mailto:kyrylo.semenchenko@gmail.com)
- **GitHub**: [Visit my GitHub profile](https://github.com/ksemk)
- **LinkedIn**: [Connect with me on LinkedIn](https://www.linkedin.com/in/kyrylo-semenchenko/)

I look forward to hearing from you!

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
