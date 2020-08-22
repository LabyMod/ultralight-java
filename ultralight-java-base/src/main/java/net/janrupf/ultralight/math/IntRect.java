package net.janrupf.ultralight.math;

import net.janrupf.ultralight.annotation.NativeType;

import java.lang.annotation.Native;
import java.util.Objects;

/**
 * Integer Rectangle Helper.
 */
@NativeType("ultralight::IntRect")
public final class IntRect {
    @Native
    private int left;
    @Native
    private int top;
    @Native
    private int right;
    @Native
    private int bottom;

    /**
     * Creates a new {@link IntRect} with all coordinates set to 0.
     */
    public IntRect() {
        this.left = 0;
        this.top = 0;
        this.right = 0;
        this.bottom = 0;
    }

    /**
     * Creates a new {@link IntRect}.
     *
     * @param left   The left coordinate of the rectangle
     * @param top    The top coordinate of the rectangle
     * @param right  The right coordinate of the rectangle
     * @param bottom The bottom coordinate of the rectangle
     */
    public IntRect(int left, int top, int right, int bottom) {
        this.left = left;
        this.top = top;
        this.right = right;
        this.bottom = bottom;
    }

    /**
     * Retrieves the width of this {@link IntRect}.
     *
     * @return The width of this rectangle
     */
    public int width() {
        return right - left;
    }

    /**
     * Retrieves the height of this {@link IntRect}.
     *
     * @return The height of this rectangle
     */
    public int height() {
        return bottom - top;
    }

    /**
     * Retrieves the left coordinate of this {@link IntRect}.
     *
     * @return The x coordinate of this rectangle
     */
    public int x() {
        return left;
    }

    /**
     * Retrieves the upper coordinate of this {@link IntRect}.
     *
     * @return The y coordinate of this rectangle
     */
    public int y() {
        return top;
    }

    /**
     * Returns x coordinate in the center of this {@link IntRect}.
     *
     * @return The center x coordinate
     */
    public int centerX() {
        return (int) (Math.round(left + right) * 0.5f);
    }

    /**
     * Returns y coordinate in the center of this {@link IntRect}.
     *
     * @return The center y coordinate
     */
    public int centerY() {
        return (int) (Math.round(top + bottom) * 0.5f);
    }

    /**
     * Checks whether this {@link IntRect} is valid.
     *
     * @return Whether this rectangle is valid
     */
    public boolean isValid() {
        return width() > 0 && height() > 0;
    }

    /**
     * Insets this {@link IntRect} by the given insets.
     *
     * @param dx The x inset
     * @param dy The y inset
     */
    public void inset(int dx, int dy) {
        this.left += dx;
        this.top += dy;
        this.right -= dx;
        this.bottom -= dy;
    }

    /**
     * Outsets this {@link IntRect} by the given insets.
     *
     * @param dx The x outset
     * @param dy The y outset
     */
    public void outset(int dx, int dy) {
        inset(-dx, -dy);
    }

    /**
     * Moves this {@link IntRect} by the given offset.
     *
     * @param dx The x offset
     * @param dy The y offset
     */
    public void move(int dx, int dy) {
        this.left += dx;
        this.top += dy;
        this.right += dx;
        this.bottom += dy;
    }

    /**
     * Checks if this is an empty {@link IntRect} (all coordinates are 0).
     *
     * @return Whether this rectangle is empty
     */
    private boolean isEmpty() {
        return this.left == 0 && this.top == 0 && this.right == 0 && this.bottom == 0;
    }

    /**
     * Joins this {@link IntRect} with another {@link IntRect} by expanding this {@link IntRect}.
     *
     * @param other The {@link IntRect} to join with
     */
    public void join(IntRect other) {
        if (isEmpty()) {
            this.left = other.left;
            this.top = other.top;
            this.right = other.right;
            this.bottom = other.bottom;
        } else {
            if (other.left < this.left) {
                this.left = other.left;
            }

            if (other.top < this.top) {
                this.top = other.top;
            }

            if (other.right > this.right) {
                this.right = other.right;
            }

            if (other.bottom > this.bottom) {
                this.bottom = other.bottom;
            }
        }
    }

    /**
     * Checks if this {@link IntRect} contains another {@link IntRect}.
     *
     * @param other The rectangle to check for if it is contained in this rectangle
     * @return Whether this rectangle contains the other rectangle
     */
    public boolean contains(IntRect other) {
        return this.left <= other.left &&
                this.top <= other.top &&
                this.right >= other.right &&
                this.bottom >= other.bottom;
    }

    /**
     * Checks if this {@link IntRect} intersects with another {@link IntRect}.
     * <b>This is used for pixel operations, so only intersections with a width and
     * height &gt;= count as intersection!</b>
     *
     * @param other The rectangle to check for intersection with this one
     * @return Whether this rectangle intersects with another rectangle
     */
    public boolean intersects(IntRect other) {
        return !(other.left >= this.right - 1 ||
                other.right < this.left ||
                other.top >= this.bottom - 1 ||
                other.bottom < this.top);
    }

    /**
     * Calculates the intersection of this {@link IntRect} with another {@link IntRect}.
     * This method only works correctly when {@link}
     *
     * @param other The rectangle this rectangle intersects with
     * @return The intersection of this rectangle with the other rectangle
     */
    public IntRect intersect(IntRect other) {
        return new IntRect(
                Math.max(this.left, other.left),
                Math.max(this.top, other.top),
                Math.min(other.right, this.right),
                Math.min(other.bottom, this.bottom)
        );
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof IntRect)) return false;
        IntRect intRect = (IntRect) o;
        return left == intRect.left &&
                top == intRect.top &&
                right == intRect.right &&
                bottom == intRect.bottom;
    }

    @Override
    public int hashCode() {
        return Objects.hash(left, top, right, bottom);
    }
}
