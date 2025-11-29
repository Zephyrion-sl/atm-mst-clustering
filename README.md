Here’s a **clean, professional, GitHub-ready `README.md`** for your ATM MST Clustering project.
It’s written in a **neutral, formal, industry-standard style** (as required).

You can copy–paste this directly into your repo.

---

# **ATM Geo-Clustering Using Minimum Spanning Tree (MST)**

A C-based geo-clustering system that groups ATMs into optimal service zones using **Kruskal’s MST algorithm**, **Haversine distance**, and **Disjoint Set Union (DSU)**.
The project helps banks reduce **cash-replenishment travel distance**, optimize **route planning**, and detect **dense or isolated ATM regions**.

---

## **1. Features**

* Computes pairwise geographical distances (Haversine formula)
* Builds a complete weighted graph of ATM locations
* Generates a **Minimum Spanning Tree** using Kruskal’s algorithm
* Automatically splits MST into **K clusters** by removing the largest edges
* Assigns each ATM to a cluster using DSU
* Estimates cluster route lengths
* Generates CSV output containing clustered ATMs
* Provides operational recommendations for banks
* Supports command-line flags
* Optional map visualization via Python + Folium

---

## **2. Use Cases**

* Optimize ATM cash-van routes
* Detect isolated/outlier ATMs
* Identify dense ATM pockets needing dedicated vehicles
* Measure total network spread
* Assist operational planning teams

---

## **3. Directory Structure**

```
project/
│
├── src/
│   ├── main.c
│   ├── atm.h / atm.c
│   ├── dsu.h / dsu.c
│   ├── kruskal.h / kruskal.c
│   ├── distance.h / distance.c
│   ├── io.h / io.c
│
├── data/
│   └── sample_input_atm.csv
│
├── tools/
│   └── visualize.py         # Python + folium map renderer
│
└── README.md
```

---

## **4. Input Format**

The input file must be a CSV with the following header:

```
atm_id,lat,lon
ATM001,17.385044,78.486671
ATM002,17.447600,78.508000
...
```

---

## **5. How to Build (GCC)**

Compile all `.c` files:

```
gcc -O2 -Wall -std=c11 src/main.c src/kruskal.c src/dsu.c src/io.c src/distance.c src/atm.c -o atm_cluster -lm
```

---

## **6. How to Run**

```
./atm_cluster --input data/sample_input_atm.csv --clusters 5 --output result_clusters.csv
```

Parameters:

* `--input` : ATM CSV file
* `--clusters` : desired number of clusters (K)
* `--output` : where output CSV should be saved

---

## **7. Sample Output (Summary)**

```
--- CLUSTER SUMMARY ---
Clusters requested: 5
Cluster 0: 3 ATMs, approx route length: 2.890 km
Cluster 1: 8 ATMs, approx route length: 13.196 km
Cluster 2: 2 ATMs, approx route length: 2.381 km
Cluster 3: 2 ATMs, approx route length: 1.517 km
Cluster 4: 5 ATMs, approx route length: 6.723 km
Total MST length: 36.844 km

Recommendations:
- Cluster 1 is dense (>=4 ATMs). Assign dedicated replenishment vehicle.
- Cluster 1 route is long (>6km). Consider splitting into subzones.
- Cluster 4 is dense (>=4 ATMs). Assign dedicated replenishment vehicle.
- Cluster 4 route is long (>6km). Consider splitting into subzones.
- Check pairs with distance < 0.2 km for redundancy (possible merge).
```

---

## **8. Output CSV Format**

`result_clusters.csv`:

```
atm_id,lat,lon,cluster
ATM001,17.385044,78.486671,0
ATM002,17.447600,78.508000,1
...
```

---

## **9. Visualizing the Clusters (Python Optional)**

Install dependencies:

```
pip install folium
```

Run map generator:

```
python tools/visualize.py result_clusters.csv
```

This produces:

```
map_clusters.html
```

Open it in any browser to see color-coded ATM clusters.

---

## **10. Algorithms Used**

### **Minimum Spanning Tree – Kruskal’s Algorithm**

* Sort edges by weight
* Greedily add edges
* Use DSU for cycle detection

### **Disjoint Set Union (Union–Find)**

* Path compression
* Union by rank

### **Haversine Distance**

* Accurate spherical distance between two lat-lon points

### **MST-Based Clustering**

* Sort MST edges descending
* Remove `k-1` largest edges
* Each connected component becomes a cluster

---

## **11. Performance**

| Component          | Complexity  |
| ------------------ | ----------- |
| Edge generation    | O(n²)       |
| Sorting edges      | O(n² log n) |
| Kruskal MST        | O(n² α(n))  |
| DSU operations     | Almost O(1) |
| Cluster assignment | O(n)        |

Handles 200–300 ATMs comfortably in real time.

---

## **12. Requirements**

* GCC / Clang
* Python 3 (optional, for visualization)
* `folium` (optional)

---

## **13. Future Improvements**

* Faster nearest-neighbor graph to avoid O(n²) edge generation
* Integrate vehicle-routing solver (VRP)
* Add clustering heatmaps
* REST API for deployment
* Live ATM feeds

---



