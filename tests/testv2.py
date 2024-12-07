import os
import random
import json
import string

import re
from pathlib import Path
import subprocess

# Directory to store test files
TEST_DIR = "../"
COMPARATIVE_FILES_DIR = "../ComparativeFiles"
RESULTS_DIR = "../Results"

# Create necessary directories
Path(TEST_DIR).mkdir(exist_ok=True)
Path(COMPARATIVE_FILES_DIR).mkdir(exist_ok=True)
Path(RESULTS_DIR).mkdir(exist_ok=True)

# Generate random content for comparative files
def generate_comparative_files(num_files=5, file_size=10000):
    comparative_files = []
    for i in range(num_files):
        file_path = Path(COMPARATIVE_FILES_DIR) / f"comp_file_{i}.txt"
        with open(file_path, "w") as f:
            content = ''.join(random.choices("abcdefghijklmnopqrstuvwxyz ", k=file_size))
            f.write(content)
        comparative_files.append(file_path)
    return comparative_files

# Generate a file to be compared and track data
def generate_file_to_compare(output_file, comparative_files, noise_ratio=0.5):
    """
    Generates a file containing content copied from comparative files and random noise.

    Args:
        output_file (Path): The path where the file to compare will be saved.
        comparative_files (list[Path]): List of comparative file paths.
        noise_ratio (float): The proportion of noise in the generated file (0.0 to 1.0).

    Returns:
        list[dict]: Tracking data for copied segments (file, start, length, etc.).
    """
    tracking_data = []
    total_length = 0

    def generate_random_noise(length):
        """Generate a random string of alphanumeric characters of given length."""
        return ''.join(random.choices(string.ascii_letters + string.digits + " ", k=length))

    with open(output_file, "w") as output:
        for _ in range(random.randint(5, 15)):  # Generate between 5-15 segments
            if random.random() < noise_ratio:
                # Add noise segment
                noise_length = random.randint(50, 200)
                noise = generate_random_noise(noise_length)
                output.write(noise)
                total_length += noise_length
            else:
                # Copy content from a random comparative file
                comp_file = random.choice(comparative_files)
                with open(comp_file, "r") as cf:
                    comp_content = cf.read()
                    comp_clean_content = re.sub(r'[^a-zA-Z0-9]', '', comp_content)
                if len(comp_clean_content) > 0:
                    start = random.randint(0, len(comp_clean_content) - 1)
                    length = random.randint(20, min(100, len(comp_clean_content) - start))
                    copied_segment = comp_clean_content[start:start + length]

                    # Insert random noise inside copied content
                    if random.random() < 0.5 and length > 10:  # Only add noise if length > 10
                        noise_insertion_point = random.randint(1, length - 1)
                        noise = generate_random_noise(random.randint(10, 150))
                        copied_segment = (
                            copied_segment[:noise_insertion_point] +
                            noise +
                            copied_segment[noise_insertion_point:]
                        )
                        length += len(noise)

                    output.write(copied_segment)
                    total_length += length

                    # Track this copied segment
                    tracking_data.append({
                        "file": comp_file.name,
                        "comp_start": start,
                        "output_start": total_length - length,
                        "output_length": length
                    })

    return tracking_data

def merge_segments(segments):
    """
    Merges overlapping or adjacent segments.
    Args:
        segments (list[dict]): List of segments with 'start' and 'length'.
    Returns:
        list[dict]: List of merged segments.
    """
    if not segments:
        return []

    # Sort segments by starting position
    segments = sorted(segments, key=lambda x: x["start"])
    merged = [segments[0]]

    for segment in segments[1:]:
        last = merged[-1]
        if segment["start"] <= last["start"] + last["length"]:  # Overlapping or adjacent
            # Merge segments
            last["length"] = max(last["start"] + last["length"], segment["start"] + segment["length"]) - last["start"]
        else:
            merged.append(segment)

    return merged


def generate_expected_json(tracking_data, expected_json_path):
    # Read the generated file to calculate total alphanumeric length
    with open("../File", "r") as f:
        file_content = f.read()
    total_clean_length = len(re.sub(r'[^a-zA-Z0-9]', '', file_content))  # Total clean length of the file

    # Merge overlapping segments
    positions = [
        {"start": item["output_start"], "length": item["output_length"]}
        for item in tracking_data
    ]
    merged_segments = merge_segments(positions)

    # Calculate total copied length from merged segments
    total_copied_length = sum(segment["length"] for segment in merged_segments)

    # Aggregate details by file
    aggregated_details = {}
    for item in tracking_data:
        file_name = item["file"]
        if file_name not in aggregated_details:
            aggregated_details[file_name] = {
                "length": 0,
                "percentage": 0.0
            }
        # Add only the portion of the segment that contributes to the unique copied length
        aggregated_details[file_name]["length"] += item["output_length"]

    # Calculate percentage for each file
    for file_name, data in aggregated_details.items():
        data["percentage"] = (data["length"] / total_clean_length) * 100

    details = [
        {
            "file": file,
            "length": data["length"],
            "percentage": data["percentage"]
        }
        for file, data in aggregated_details.items()
    ]

    # Calculate total plagiarism percentage
    total_percentage = (total_copied_length / total_clean_length) * 100

    summary = {
        "total_percentage": total_percentage,
        "positions": merged_segments,
        "details": details
    }

    # Save expected JSON file
    with open(expected_json_path, "w") as expected_file:
        json.dump(summary, expected_file, indent=4)

    print(f"Expected JSON file generated: {expected_json_path}")

def generate_and_run_tests(executable, k_values, w_values, algorithms, modulo_values):
    comparative_files = generate_comparative_files()
    output_file = Path(TEST_DIR) / "File"

    # Generate the file to compare and track copied content
    tracking_data = generate_file_to_compare(output_file, comparative_files)

    tested_configurations = set()

    # Generate the expected JSON file
    expected_json_path = Path(RESULTS_DIR) / "expected_results.json"
    generate_expected_json(tracking_data, expected_json_path)

    for algo in algorithms:
        for k in k_values:
            if algo == "winnowing":
                for w in w_values:
                    # Ignore modulo for winnowing
                    config = (algo, k, w, None)
                    if config not in tested_configurations:
                        tested_configurations.add(config)
                        run_app(executable, k, w, algo, modulo=0)  # Modulo is irrelevant
            elif algo == "modulo":
                for modulo in modulo_values:
                    # Ignore W for modulo
                    config = (algo, k, None, modulo)
                    if config not in tested_configurations:
                        tested_configurations.add(config)
                        run_app(executable, k, w=0, algo=algo, modulo=modulo) 

# Run the app with given parameters
def run_app(executable, k, w, algo, modulo):
    command = [
        executable,
        "--K", str(k),
        "--W", str(w),
        "--algorithm", algo,
        "--modulo", str(modulo),
    ]
    print(f"Running command: {' '.join(command)}")
    result = subprocess.run(command, check=False, capture_output=True, text=True)

    # Log stdout and stderr for debugging
    print(f"App stdout: {result.stdout}")
    print(f"App stderr: {result.stderr}")

# Execute the script
if __name__ == "__main__":
    # Example parameters
    executable = "./myExe"  # Path to your app
    k_values = [i for i in range (5, 20)]  # K-gram lengths
    w_values = [i for i in range (4, 15)]  # Window lengths
    algorithms = ["modulo", "winnowing"]  # Algorithms to test
    modulo_values = [i for i in range (5, 15)]  # Modulo values for 'modulo' algorithm

    generate_and_run_tests(executable, k_values, w_values, algorithms, modulo_values)

# Example usage:
# The generate_and_run_tests() function remains unchanged and will now use the corrected `generate_expected_json`.
