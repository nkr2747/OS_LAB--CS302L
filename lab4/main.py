import cv2
import numpy as np
import random
import os

def generate_wire_patch(size=64, num_lines=1, thickness=1):
    """
    Generate a synthetic wire-like patch: thin black lines on white background.
    """
    img = np.ones((size, size), dtype=np.uint8) * 255  # white background

    for _ in range(num_lines):
        # Random line endpoints
        x1, y1 = random.randint(0, size-1), random.randint(0, size-1)
        x2, y2 = random.randint(0, size-1), random.randint(0, size-1)

        # Draw line
        cv2.line(img, (x1, y1), (x2, y2), (0,), thickness)

    return img

def generate_dataset(out_dir="wire_patches", n=1000, size=64):
    os.makedirs(out_dir, exist_ok=True)
    for i in range(n):
        num_lines = random.choice([1, 2, 3])         # sometimes crossing wires
        thickness = random.choice([1, 2])            # thin or slightly thicker
        patch = generate_wire_patch(size, num_lines, thickness)
        cv2.imwrite(os.path.join(out_dir, f"wire_{i}.png"), patch)

# Example usage
generate_dataset("wire_patches", n=5000, size=64)
