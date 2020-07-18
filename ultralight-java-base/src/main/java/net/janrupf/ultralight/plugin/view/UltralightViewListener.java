package net.janrupf.ultralight.plugin.view;

/**
 * Interface for View-related events
 */
public interface UltralightViewListener {
    /**
     * Called when the page title changes
     *
     * @param title The new page title
     */
    void onChangeTitle(String title);

    /**
     * Called when the page URL changes
     *
     * @param url The new page url
     */
    void onChangeURL(String url);

    /**
     * Called when the tooltip changes (usually as result of a mouse hover)
     *
     * @param tooltip The new page tooltip
     */
    void onChangeTooltip(String tooltip);
}
