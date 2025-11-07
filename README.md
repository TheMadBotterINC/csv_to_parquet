# CSV to Parquet Converter

A fast, lightweight command-line tool for converting CSV files to Parquet format using DuckDB.

## Overview

This tool leverages DuckDB's high-performance CSV reader and Parquet writer to efficiently convert CSV files to the columnar Parquet format. It automatically infers data types and handles complex CSV features like quoted fields, embedded commas, and special characters.

## Features

- **Automatic type inference**: DuckDB intelligently detects column types (integers, floats, dates, strings, etc.)
- **Handles complex CSVs**: Supports quoted fields, embedded newlines, commas within quotes, and special characters
- **Fast and efficient**: Uses DuckDB's optimized CSV parser and Parquet writer
- **Simple CLI**: Just provide input and output file paths
- **SQL injection protection**: File paths are properly escaped for safe SQL execution

## Prerequisites

- DuckDB C library installed
- C compiler (gcc, clang, etc.)
- macOS, Linux, or Unix-like environment

## Building

Compile the tool with the DuckDB library:

```bash
gcc -o csv_to_parquet csv_to_parquet.c -lduckdb
```

Or with optimization flags:

```bash
gcc -O3 -o csv_to_parquet csv_to_parquet.c -lduckdb
```

Make sure the DuckDB library is in your library path, or specify the path explicitly:

```bash
gcc -o csv_to_parquet csv_to_parquet.c -I/path/to/duckdb/include -L/path/to/duckdb/lib -lduckdb
```

## Usage

```bash
./csv_to_parquet input.csv output.parquet
```

### Examples

Convert a simple CSV file:
```bash
./csv_to_parquet data.csv data.parquet
```

Convert with full paths:
```bash
./csv_to_parquet /path/to/input.csv /path/to/output.parquet
```

## How It Works

1. Opens an in-memory DuckDB database
2. Uses `read_csv_auto()` to automatically parse the CSV with proper type inference
3. Writes the data to a Parquet file using DuckDB's `COPY` command
4. Properly escapes file paths to prevent SQL injection vulnerabilities

The tool assumes CSV files have a header row. DuckDB's `read_csv_auto()` function automatically:
- Detects delimiters
- Infers column types
- Handles quoted fields
- Processes escape characters
- Manages multiline values

## Testing

A comprehensive test suite is included to verify functionality:

```bash
./test_csv_to_parquet.sh
```

The test suite covers:
- Basic CSV to Parquet conversion
- Empty CSV files
- Data type mapping (integers, floats, strings, dates)
- Non-existent input files
- Special characters and complex quoting
- Large file handling
- Missing command-line arguments

## Advanced Configuration

The code includes a comment about enabling `ALL_VARCHAR=TRUE` mode. If you need all columns treated as strings (safer but less optimized), modify line 60:

```c
"COPY (SELECT * FROM read_csv_auto('%s', HEADER=TRUE, ALL_VARCHAR=TRUE)) "
```

## Error Handling

The tool provides clear error messages for common issues:
- Missing command-line arguments
- Failed to open DuckDB
- Failed database connection
- CSV parsing errors
- Parquet writing errors

Exit codes:
- `0`: Success
- `1`: Error occurred (with descriptive message to stderr)

## Performance

This tool is highly efficient for CSV to Parquet conversion:
- Minimal memory footprint
- Streaming processing for large files
- No intermediate storage required
- DuckDB's optimized I/O operations

## Limitations

- File paths are limited to 2048 characters
- Assumes CSV files have headers
- Requires DuckDB library to be installed

## License

This is an internal tool. Check with your organization for licensing terms.

## Contributing

When contributing, please:
- Run the test suite before submitting changes
- Follow existing code style
- Update tests for new features
- Document any new functionality

## Troubleshooting

**"Failed to open DuckDB"**: Ensure the DuckDB library is properly installed and accessible.

**"Query failed"**: Check that:
- Input CSV file exists and is readable
- Output directory is writable
- CSV file is properly formatted

**Compilation errors**: Verify DuckDB headers and libraries are in the correct paths.
