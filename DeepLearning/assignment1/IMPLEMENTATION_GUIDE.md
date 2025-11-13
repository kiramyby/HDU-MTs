# Assignment 1 Implementation Guide

This guide walks through the key implementations added to the assignment codebase. Each section explains the main ideas, design decisions, and guidance for further study.

## 1. k-Nearest Neighbor (`cs231n/classifiers/k_nearest_neighbor.py`)

- **Distance Computation:**
  - `compute_distances_two_loops`, `compute_distances_one_loop`, and `compute_distances_no_loops` progressively eliminate loops using broadcasting. The fully vectorized version relies on the identity \( \|x - y\|^2 = \|x\|^2 + \|y\|^2 - 2x^Ty \).
  - `np.maximum(…, 0.0)` guards against negative rounding errors before taking square roots.
- **Label Prediction:**
  - For each test point, `np.argsort` selects the indices of the `k` nearest neighbors. `np.bincount` tallies votes so ties fall toward the smaller label index.

## 2. Core Layers (`cs231n/layers.py`)

- **Affine + ReLU Blocks:**
  - Forward passes reshape inputs to row vectors, apply matrix multiplication, and add bias. ReLU uses `np.maximum(0, x)`.
  - Backward passes leverage cached inputs to propagate gradients via standard calculus: upstream gradients back through matrix multiplies, reshape to original input shape, and mask ReLU gradients where activations were negative.
- **Normalization Layers:**
  - **Batch Norm:** Computes per-feature minibatch statistics during training, updates running estimates, and normalizes inputs before applying learned scale (`gamma`) and shift (`beta`). Backward pass decomposes gradients across mean and variance branches.
  - **Layer Norm:** Mirrors batch norm but normalizes across features for each data point, making training and test behavior identical.
  - **Spatial Batch/Group Norm:** Reshape tensors to reuse batch / layer norm logic along channel dimensions. Group norm splits channels into groups to balance batch-independent normalization.
- **Dropout:**
  - Implements *inverted dropout* by scaling kept activations by `1/p` during training so no scaling is needed at test time.
- **Convolution & Pooling:**
  - Naive implementations slide filter windows explicitly. Gradients accumulate into weight derivatives and the padded input tensor, then crop padding.
  - Max pooling stores argmax masks to route upstream gradients.
- **Losses:**
  - Multiclass SVM computes hinge losses, while softmax uses stabilized exponentials. Both return analytic gradients for backprop.

## 3. Optimization Algorithms (`cs231n/optim.py`)

- **SGD with Momentum:** Maintains a velocity term that exponentially averages gradients, smoothing updates.
- **RMSProp:** Tracks a decaying average of squared gradients per parameter to adapt learning rates.
- **Adam:** Combines momentum (first moment) and adaptive scaling (second moment) with bias correction for robust convergence.

## 4. Network Architectures (`cs231n/classifiers/fc_net.py`)

- **TwoLayerNet:**
  - Assembles an affine–ReLU hidden layer, affine output layer, and softmax loss. Gradients include L2 regularization (with 0.5 factor for symmetric gradients).
- **FullyConnectedNet:**
  - Builds arbitrary-depth networks with optional batch/layer normalization and dropout.
  - Forward pass chains affine, normalization, ReLU, and dropout layers, storing caches for each.
  - Backward pass unwinds the chain: apply softmax gradient, backprop through the final affine layer, then iterate hidden layers in reverse, handling dropout and normalization when enabled.
  - Regularization sums \(\lambda/2 \sum \|W_i\|^2\) and adds \(\lambda W_i\) to each weight gradient.

## 5. Suggested Study Checklist

1. **Matrix Identities:** Re-derive the vectorized L2 distance formula to build intuition for broadcasting.
2. **Computation Graphs:** Sketch forward/backward graphs for batch norm and layer norm; label intermediate gradients to validate the implementation.
3. **Dropout Scaling:** Confirm why inverted dropout keeps expected activation magnitudes unchanged between training and testing.
4. **Conv/Pool Gradients:** Manually compute gradients on a tiny 1D example to verify the indexing logic used in the naive loops.
5. **Regularization Gradients:** Show that differentiating \(\frac{1}{2}\lambda \|W\|^2\) yields \(\lambda W\), explaining the 0.5 factor used throughout.
6. **Optimizer Dynamics:** Experiment with simple quadratic loss surfaces to visualize how momentum, RMSProp, and Adam trajectories differ from vanilla SGD.

Understanding these components deeply will make debugging and extending future assignments much easier. Feel free to adapt or extend this guide with personal notes as you explore further experiments.
