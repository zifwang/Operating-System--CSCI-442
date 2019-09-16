#pragma once


/**
 * Represents a CPU or IO burst.
 */
struct Burst {
  /**
   * The type of the burst.
   */
  enum Type {
    CPU,
    IO
  };

  /**
   * The type of burst.
   */
  Type type;

  /**
   * The length of the burst.
   */
  int length;

  /**
   * Creates a burst of the given type and length.
   */
  Burst(Type type, int length) : type(type), length(length) {}
};
