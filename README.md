# CellularArtist
![ezgif-1-1a0ae4b8c7](https://github.com/IoneIvan/CellularArtist/assets/110576193/ae7dbc67-6b10-4672-b0f9-9167bc614b20)

Each cell has genes (rgb) and color (rgb)

First:

    Update Rule: The color of each cell is updated based on its diagonal neighbors.

    Calculation: The calculated value is adjusted using gene values and summed up for all 9 neighbors.

    Final Result: The computed color becomes the new color for the current cell.

Second:

    Update Rule: Identifies the neighboring cell with the least error compared to the original image.

    Error Calculation: Error is computed by comparing colors of the current cell and its neighbors in both the current and original color (from prompt image).

    Selection: The neighbor with the minimum error is chosen.

    Update: The gene of the cell is then changed based on the gene value at the position of the selected neighbor with the minimum error.
