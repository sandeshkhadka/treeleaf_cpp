# Student Records and 3D Box Transformations

## Overview
This repository contains two tasks:
1. **CSV Student Records Sorting**: A program that reads student records from a CSV file and sorts the data based on name and age.
2. **3D Box Transformations**: A program that performs transformations (rotation and scaling) on a 3D box represented by its vertices.

## Assumptions and Requirement
- The input CSV file follows the format: `id,name,date_birth,roll_no,department,address`.
- For the transformations, angles should be in degrees and the scaling factor should be a positive number.

## Platform and environment
- Compiled and tested on x86_64-pc-linux-gnu with clang version 18.1.8

## Task 1: CSV Student Records Sorting

### Approach
1. **Reading CSV File**: The program reads a CSV file containing student records and parses each row to create `Student` objects.
2. **Sorting**: The records can be sorted by name or by age. Age is calculated from the date of birth.
3. **Exporting**: The sorted data is exported to new CSV files.

### Instructions
1. **Compile and Run**:
   ```bash
   clang++ -o csvsort csv_sort.cpp
   ./csvsort input.csv [sortby]
   ```
   - `input.csv`: The CSV file containing student records.
   - `[sortby]`: Sorting criteria (`age`, `name`, or `both`).
   For help:
   ```bash
   ./csvsort -h
   ```

2. **Example Usage**:
   ```bash
   ./csvsort students.csv age
   ```
   This will generate `SortedByAge.csv`.

## Task 2: 3D Box Transformations

### Approach
1. **Vertex and Box Representation**: The box is represented using `Vertex` and `Box` classes.
2. **Transformations**: The `Transformation` class performs rotations about the X, Y, and Z axes and scaling based on user input.
3. **Transformation Method**: Matrix multiplication is used for transformation

**Note: The Vertex structure has been updated to be homogenous(additional element h) for matrix multiplication. Although it is not
required unless for translation, I decided to do that so it can be easily be exented to support other
transformation in future.**

### Instructions
1. **Compile and Run**:
   ```bash
   clang++ -o transformations transformations.cpp
   ./transformations 
   ```
2. **Example Usage**:
   - Enter the transformation angle (in degrees) and scaling factor when prompted.

## Example Output
```
Original Box:
...
Rotated about X axis by 90 degrees:
...
Scaled by factor of 2:
...
```

