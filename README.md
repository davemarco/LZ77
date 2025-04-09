# LZ77

This project implements LZ77 compression (`lz77.cpp`) and decompression (`lz77d.cpp`).

## Build Instructions

Use the provided `Makefile` to build the executables:

```bash
make
```

This will generate two executables: `lz77` and `lz77d`.

## Usage

### Compression
```bash
./lz77 <input_file> <output_file> <search_buf_size> <lookahead_buf_size>
```
- `<input_file>`: Path to the file to compress.
- `<output_file>`: Path to save the compressed file.
- `<search_buf_size>`: Size of the search buffer (e.g., 4095).
- `<lookahead_buf_size>`: Size of the lookahead buffer (e.g., 15).

### Decompression
```bash
./lz77d <input_file> <output_file> <search_buf_size> <lookahead_buf_size>
```
- `<input_file>`: Path to the compressed file.
- `<output_file>`: Path to save the decompressed file.
- `<search_buf_size>`: Size of the search buffer (must match compression).
- `<lookahead_buf_size>`: Size of the lookahead buffer (must match compression).
