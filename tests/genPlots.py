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
    return actual - expected

def generate_scatter_plots(summary_files, expected_data):
    """Generate separate scatter plots for modulo and w values."""
    # Parse all summary files and extract method, params, and differences
    data_list = []
    for file_path in summary_files:
        # Load actual data
        json_data = load_json(file_path)
        data_list.append(json_data)

    # Collect data for plotting
    k_values_modulo = []
    differences_modulo = []
    labels_modulo = []  # To store modulo labels
    
    k_values_w = []
    differences_w = []
    labels_w = []  # To store w labels

    for item in data_list:
        k = item["params"]["k"]
        total_plagiarism = item["total_plagiarism"]
        total_percentage = expected_data.get("total_percentage", 0)

        # Calculate the difference
        diff = calculate_difference(total_plagiarism, total_percentage)

        # Separate the data into w and modulo categories
        if item["params"]["w"] != 0:
            label = item['params']['w']
            k_values_w.append(k)
            differences_w.append(diff)
            labels_w.append(label)
        else:
            label = item['params']['modulo']
            k_values_modulo.append(k)
            differences_modulo.append(diff)
            labels_modulo.append(label)

    # Plot for modulo values
    plt.figure(figsize=(10, 6))
    plt.scatter(k_values_modulo, differences_modulo, c='blue', alpha=0.7)

    # Add labels for modulo points
    for i, label in enumerate(labels_modulo):
        plt.text(k_values_modulo[i], differences_modulo[i], label, fontsize=9, ha='right')

    # Set plot titles and labels
    plt.title("Scatter Plot of Total Plagiarism Difference vs k (Modulo)")
    plt.xlabel("k values")
    plt.ylabel("Difference between Total Plagiarism and Total Percentage")
    
    # Save the modulo plot
    plot_filename_modulo = os.path.join("../Results/plots/", "scatter_plot_modulo.png")
    plt.savefig(plot_filename_modulo)
    plt.close()  # Close the figure to free memory

    # Plot for w values
    plt.figure(figsize=(10, 6))
    plt.scatter(k_values_w, differences_w, c='green', alpha=0.7)

    # Add labels for w points
    for i, label in enumerate(labels_w):
        plt.text(k_values_w[i], differences_w[i], label, fontsize=9, ha='right')

    # Set plot titles and labels
    plt.title("Scatter Plot of Total Plagiarism Difference vs k (w values)")
    plt.xlabel("k values")
    plt.ylabel("Difference between Total Plagiarism and Total Percentage")
    
    # Save the w plot
    plot_filename_w = os.path.join("../Results/plots/", "scatter_plot_w.png")
    plt.savefig(plot_filename_w)
    plt.close()  # Close the figure to free memory

def main():
    # Find all summary files
    summary_files = find_summary_files("../Results")
    print(f"Found {len(summary_files)} 'SummaryResults.json' files.")
    
    # Load expected results
    expected_data = load_json("../Results/expected_results.json")

    # Generate the scatter plot comparisons for modulo and w values
    generate_scatter_plots(summary_files, expected_data)

if __name__ == "__main__":
    main()
