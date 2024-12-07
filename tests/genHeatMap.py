import os
import json
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

def load_json(file_path):
    """Load JSON data from a file."""
    with open(file_path, 'r') as file:
        return json.load(file)

def find_summary_files(directory):
    """Recursively find all 'SummaryResults.json' files in a directory."""
    summary_files = []
    for root, _, files in os.walk(directory):
        for file in files:
            if file == "SummaryResults.json":
                summary_files.append(os.path.join(root, file))
    return summary_files

def calculate_difference(actual, expected):
    """Calculate the absolute difference between actual and expected results."""
    actual_total = actual
    expected_total = expected
    return actual_total - expected_total

def generate_heatmap(summary_files, expected_data):
    """Generate a heatmap comparing the difference between actual and expected results."""
    # Parse all summary files and extract method, params
    data_list = []
    for file_path in summary_files:
        # Load actual data
        json = load_json(file_path)
        data_list.append(json)

    # Create a grid for the heatmap, using k for rows and the other param (w or modulo) for columns
    # print(data_list)
    k_values = sorted(set(item["params"]["k"] for item in data_list))
    modulo_values = sorted(set(item["params"]["modulo"] for item in data_list if item["params"]["modulo"] != 0))
    w_values = sorted(set(item["params"]["w"] for item in data_list if item["params"]["w"] != 0))
    w_heatmap_data = np.zeros((len(w_values), len(k_values)))
    modulo_heatmap_data = np.zeros((len(modulo_values), len(k_values)))
    for item in data_list:
        k = item["params"]["k"]
        if item["params"]["w"] != 0:
            w = item["params"]["w"]
            k_index = k_values.index(k)
            w_index = w_values.index(w)
            total_plagiarism = item["total_plagiarism"]
            w_heatmap_data[w_index][k_index] = calculate_difference(total_plagiarism, expected_data.get("total_percentage"))
        else:
            modulo = item["params"]["modulo"]
            k_index = k_values.index(k)
            modulo_index = modulo_values.index(modulo)
            total_plagiarism = item["total_plagiarism"]
            modulo_heatmap_data[modulo_index][k_index] = calculate_difference(total_plagiarism, expected_data.get("total_percentage"))
    # Decide which parameter to use for the other axis
    # If any w values exist, use w as the second axis, else use modulo
    # Initialize a matrix to store total_plagiarism values

    # Fill the matrix with total_plagiarism values


    # Plotting the heatmap
    # print(w_heatmap_data)
    plt.figure(figsize=(10, 6))
    sns.heatmap(w_heatmap_data, xticklabels=k_values, yticklabels=w_values, cmap="YlGnBu", cbar_kws={'label': 'Total Plagiarism'})
    plt.title("Heatmap of Total Plagiarism (k vs w)")
    plt.xlabel("k values")
    plt.ylabel("w values")
    plot_filename = os.path.join("../Results/plots/", "heatmap_comparison_winnowing.png")
    plt.savefig(plot_filename)
    plt.close()  # Close the figure to free memory
    

    plt.figure(figsize=(10, 6))
    sns.heatmap(modulo_heatmap_data, xticklabels=k_values, yticklabels=modulo_values, cmap="YlGnBu", cbar_kws={'label': 'Total Plagiarism'})
    plt.title("Heatmap of Total Plagiarism (k vs modulo)")
    plt.xlabel("k values")
    plt.ylabel("modulo values")
    plot_filename = os.path.join("../Results/plots/", "heatmap_comparison_modulo.png")
    plt.savefig(plot_filename)
    plt.close()  # Close the figure to free memory
    
    # Save the heatmap plot



def main():
    # Find all summary files
    summary_files = find_summary_files("../Results")
    print(f"Found {len(summary_files)} 'SummaryResults.json' files.")
    
    # Load expected results
    expected_data = load_json("../Results/expected_results.json")

    # Generate the heatmap comparison plot
    generate_heatmap(summary_files, expected_data)

if __name__ == "__main__":
    main()
