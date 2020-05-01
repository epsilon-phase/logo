#pragma once
namespace logo {
  namespace __detail {
    /// Correctness bounds for floating point equality comparisons
    /// Since we still haven't gotten around to that one paper about
    /// "what all computer scientists should know about floating point".
    const float EPSILON = 1E-6f;
  } // namespace __detail
} // namespace logo