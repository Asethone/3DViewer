#include "gui.h"

GtkWindow *window, *window_preferences;
GtkDrawingArea *area;
GtkBuilder *build, *preferences_build;
static GtkEntryBuffer *eb_x_translate, *eb_y_translate, *eb_z_translate;
static GtkEntryBuffer *eb_x_rotate, *eb_y_rotate, *eb_z_rotate;
// static Gtk
settings_t options;
bool isScreenshotTime;
bool isScreencastTime;
int screencastTimer;

// SCREENSHOT
void takeScreenshot() {
    isScreenshotTime = true;
    gtk_widget_queue_draw(GTK_WIDGET(area));
}

// SCREENCAST
gpointer threadScreencast(gpointer data) {
    struct timeval start;
    screencastTimer = 1;
    while (screencastTimer <= 50) {
        gettimeofday(&start, NULL);
        if (start.tv_usec % 100000 == 0) {
            isScreencastTime = true;
            gtk_widget_queue_draw(GTK_WIDGET(area));
            screencastTimer++;
        }
    }
    system("convert -delay 10 -loop 0 " USER_APP "screencasts/*.png " USER_APP "screencasts/screencast.gif");
    system("rm -f " USER_APP "screencasts/*.png");
}

void takeScreencast() { g_thread_new("screencast", threadScreencast, NULL); }

// PREFERENCES
void savePreferences_gui() {
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(gtk_builder_get_object(preferences_build, "cb_edges_color")),
                                options.e_color == WHITE    ? "edges_white"
                                : options.e_color == CYAN   ? "edges_cyan"
                                : options.e_color == GOLDEN ? "edges_golden"
                                                            : NULL);
    gtk_spin_button_set_value(
        GTK_SPIN_BUTTON(gtk_builder_get_object(preferences_build, "sb_edges_thickness")),
        options.e_thickness);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(gtk_builder_get_object(preferences_build, "cb_edges_type")),
                                options.e_type == SOLID    ? "edges_solid"
                                : options.e_type == DASHED ? "edges_dashed"
                                                           : NULL);
    gtk_combo_box_set_active_id(
        GTK_COMBO_BOX(gtk_builder_get_object(preferences_build, "cb_vertices_color")),
        options.v_color == WHITE    ? "vertices_white"
        : options.v_color == CYAN   ? "vertices_cyan"
        : options.v_color == GOLDEN ? "vertices_golden"
                                    : NULL);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtk_builder_get_object(preferences_build, "sb_vertices_size")),
                              options.v_size);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(gtk_builder_get_object(preferences_build, "cb_vertices_type")),
                                options.v_shape == NONE     ? "vertices_solid"
                                : options.v_shape == CIRCLE ? "vertices_square"
                                : options.v_shape == SQUARE ? "vertices_square"
                                                            : NULL);
    gtk_combo_box_set_active_id(
        GTK_COMBO_BOX(gtk_builder_get_object(preferences_build, "cb_other_projection")),
        options.projection == PARALLEL  ? "other_projection_parallel"
        : options.projection == CENTRAL ? "other_projection_central"
                                        : NULL);
    gtk_combo_box_set_active_id(GTK_COMBO_BOX(gtk_builder_get_object(preferences_build, "cb_other_color")),
                                options.bg_color == DARK_GREY    ? "other_default"
                                : options.bg_color == LIGHT_GREY ? "other_grey"
                                : options.bg_color == BLUE       ? "other_blue"
                                                                 : NULL);
}

void openPreferences() {
    preferences_build = gtk_builder_new_from_file(USER_APP "style/preferences.xml");
    window_preferences = GTK_WINDOW(gtk_builder_get_object(preferences_build, "window"));
    savePreferences_gui();
    gtk_widget_show(GTK_WIDGET(window_preferences));
}

void closePreferences() { gtk_window_close(window_preferences); }

void savePreferences() {
    char *str = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(gtk_builder_get_object(preferences_build, "cb_edges_color")));
    options.e_color = strcmp(str, "white") == 0    ? WHITE
                      : strcmp(str, "cyan") == 0   ? CYAN
                      : strcmp(str, "golden") == 0 ? GOLDEN
                                                   : -1;
    options.e_thickness = gtk_spin_button_get_value(
        GTK_SPIN_BUTTON(gtk_builder_get_object(preferences_build, "sb_edges_thickness")));
    str = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(gtk_builder_get_object(preferences_build, "cb_edges_type")));
    options.e_type = strcmp(str, "solid") == 0 ? SOLID : strcmp(str, "dashed") == 0 ? DASHED : -1;
    str = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(gtk_builder_get_object(preferences_build, "cb_vertices_color")));
    options.v_color = strcmp(str, "white") == 0    ? WHITE
                      : strcmp(str, "cyan") == 0   ? CYAN
                      : strcmp(str, "golden") == 0 ? GOLDEN
                                                   : -1;
    options.v_size = gtk_spin_button_get_value(
        GTK_SPIN_BUTTON(gtk_builder_get_object(preferences_build, "sb_vertices_size")));
    str = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(gtk_builder_get_object(preferences_build, "cb_vertices_type")));
    options.v_shape = strcmp(str, "none") == 0     ? NONE
                      : strcmp(str, "square") == 0 ? SQUARE
                      : strcmp(str, "circle") == 0 ? CIRCLE
                                                   : -1;
    str = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(gtk_builder_get_object(preferences_build, "cb_other_projection")));
    options.projection = strcmp(str, "parallel") == 0  ? PARALLEL
                         : strcmp(str, "central") == 0 ? CENTRAL
                                                       : -1;
    str = gtk_combo_box_text_get_active_text(
        GTK_COMBO_BOX_TEXT(gtk_builder_get_object(preferences_build, "cb_other_color")));
    options.bg_color = strcmp(str, "dark grey") == 0    ? DARK_GREY
                       : strcmp(str, "light grey") == 0 ? LIGHT_GREY
                       : strcmp(str, "blue") == 0       ? BLUE
                                                        : -1;
    save_preferences_in_file(USER_APP "config/preferences.conf");
    closePreferences();
    gtk_widget_queue_draw(GTK_WIDGET(area));
}

void setPreferencesDefault() {
    init_settings(USER_APP "config/default.conf");
    save_preferences_in_file(USER_APP "config/preferences.conf");
    savePreferences_gui();
}

// BUTTONS PRESS
void pressChooseFile(GtkWidget *cbox) {
    freeData();
    const char *filename = gtk_entry_buffer_get_text(
        gtk_entry_get_buffer(GTK_ENTRY(gtk_widget_get_first_child(gtk_widget_get_first_child(cbox)))));
    viewModel(filename);
}

void pressPlus(GtkWidget *textField) {
    GtkEntryBuffer *eb = gtk_text_get_buffer(GTK_TEXT(textField));
    const char *text = gtk_entry_buffer_get_text(eb);
    char *err = NULL;
    double val = strtod(text, &err);

    if (!*text) {
        gtk_entry_buffer_set_text(eb, "1", 1);
    } else if (!*err) {
        char strVal[100];
        val += fabs(fmod(val, 1)) < 1e-7 ? 1 : ceil(val) - val;
        snprintf(strVal, sizeof(strVal), "%.f", val);
        gtk_entry_buffer_set_text(eb, strVal, strlen(strVal));
    } else {
        GtkWidget *box = gtk_widget_get_parent(gtk_widget_get_parent(textField));
        GtkWidget *btn = gtk_widget_get_last_child(box);
        const char *label = gtk_button_get_label(GTK_BUTTON(btn));
        if (*label == 'T') {
            errorMessage(W_TRANS_DATA);
        } else {
            errorMessage(W_ROT_DATA);
        }
    }
}

void pressMinus(GtkWidget *textField) {
    GtkEntryBuffer *eb = gtk_text_get_buffer(GTK_TEXT(textField));
    const char *text = gtk_entry_buffer_get_text(eb);
    char *err = NULL;
    double val = strtod(text, &err);

    if (!*text) {
        gtk_entry_buffer_set_text(eb, "-1", 2);
    } else if (!*err) {
        char strVal[100];
        val -= fabs(fmod(val, 1)) < 1e-7 ? 1 : val - floor(val);
        snprintf(strVal, sizeof(strVal), "%.f", val);
        gtk_entry_buffer_set_text(eb, strVal, strlen(strVal));
    } else {
        GtkWidget *box = gtk_widget_get_parent(gtk_widget_get_parent(textField));
        GtkWidget *btn = gtk_widget_get_last_child(box);
        const char *label = gtk_button_get_label(GTK_BUTTON(btn));
        if (*label == 'T') {
            errorMessage(W_TRANS_DATA);
        } else {
            errorMessage(W_ROT_DATA);
        }
    }
}

void pressMinusScale(GtkWidget *eb) {
    const char *text = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(eb));
    char *err = NULL;
    double val = strtod(text, &err);

    if (!*text) {
        gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(eb), "1", 1);
    } else if (!*err) {
        char strVal[100];
        val /= 1.1;
        snprintf(strVal, sizeof(strVal), "%.3g", val);
        gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(eb), strVal, strlen(strVal));
    } else {
        errorMessage(W_SCALE_DATA);
    }
}

void pressPlusScale(GtkWidget *eb) {
    const char *text = gtk_entry_buffer_get_text(GTK_ENTRY_BUFFER(eb));
    char *err = NULL;
    double val = strtod(text, &err);

    if (!*text) {
        gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(eb), "1", 1);
    } else if (!*err) {
        char strVal[100];
        val *= 1.1;
        snprintf(strVal, sizeof(strVal), "%.3g", val);
        gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(eb), strVal, strlen(strVal));
    } else {
        errorMessage(W_SCALE_DATA);
    }
}

void pressTranslate() {
    const char *text = gtk_entry_buffer_get_text(eb_x_translate);
    char *tpx, *tpy, *tpz;
    double x = strtod(text, &tpx);
    text = gtk_entry_buffer_get_text(eb_y_translate);
    double y = strtod(text, &tpy);
    text = gtk_entry_buffer_get_text(eb_z_translate);
    double z = strtod(text, &tpz);
    if (*tpx == 0 && *tpy == 0 && *tpz == 0) {
        double trans_data[3] = {x, y, z};
        viewTransformModel(trans_data, TRANSLATE);
    } else {
        errorMessage(W_TRANS_DATA);
    }
}

void pressRotate() {
    const char *text = gtk_entry_buffer_get_text(eb_x_rotate);
    char *tpx, *tpy, *tpz;
    double x = strtod(text, &tpx);
    text = gtk_entry_buffer_get_text(eb_y_rotate);
    double y = strtod(text, &tpy);
    text = gtk_entry_buffer_get_text(eb_z_rotate);
    double z = strtod(text, &tpz);
    if (*tpx == 0 && *tpy == 0 && *tpz == 0) {
        double rot_data[3] = {x, y, z};
        viewTransformModel(rot_data, ROTATE);
    } else {
        errorMessage(W_ROT_DATA);
    }
}

void pressScale(GtkEntryBuffer *eb) {
    const char *text = gtk_entry_buffer_get_text(eb);
    char *tp;
    double s = strtod(text, &tp);
    if (*tp == 0) {
        if (s == 0) s = 1;
        viewTransformModel(&s, SCALE);
    } else {
        errorMessage(W_SCALE_DATA);
    }
}

// CLOSE WINDOW REQUEST
static void closeWindow(GtkWidget *widget, gpointer data) {
    freeData();
    GApplication *app = G_APPLICATION(data);
    g_application_quit(app);
}

// CSS RULES APPLY
void myCSS() {
    GtkCssProvider *provider = gtk_css_provider_new();
    GdkDisplay *display = gdk_display_get_default();

    gtk_css_provider_load_from_file(provider, g_file_new_for_path(USER_APP "style/stylesheet.css"));

    gtk_style_context_add_provider_for_display(display, GTK_STYLE_PROVIDER(provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}

// WINDOW BUILDER
static void buildWindow(GtkApplication *app) {
    build = gtk_builder_new_from_file(USER_APP "style/gui.xml");
    init_settings(USER_APP "config/preferences.conf");
    // Window
    window = GTK_WINDOW(gtk_builder_get_object(build, "window"));
    gtk_window_set_application(window, app);

    // Drawing area
    area = GTK_DRAWING_AREA(gtk_builder_get_object(build, "area"));
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(area), draw_function, NULL, NULL);

    // Translate data
    eb_x_translate = GTK_ENTRY_BUFFER(gtk_builder_get_object(build, "eb_x_translate"));
    eb_y_translate = GTK_ENTRY_BUFFER(gtk_builder_get_object(build, "eb_y_translate"));
    eb_z_translate = GTK_ENTRY_BUFFER(gtk_builder_get_object(build, "eb_z_translate"));

    // Rotate data
    eb_x_rotate = GTK_ENTRY_BUFFER(gtk_builder_get_object(build, "eb_x_rotate"));
    eb_y_rotate = GTK_ENTRY_BUFFER(gtk_builder_get_object(build, "eb_y_rotate"));
    eb_z_rotate = GTK_ENTRY_BUFFER(gtk_builder_get_object(build, "eb_z_rotate"));

    g_signal_connect(window, "close-request", G_CALLBACK(closeWindow), app);
}

// MAIN ACTIVATE FUNCTION
static void activate(GtkApplication *app, gpointer user_data) {
    buildWindow(app);
    init_settings(USER_APP "config/preferences.conf");

    myCSS();

    gtk_widget_show(GTK_WIDGET(window));
}

int main(int argc, char **argv) {
    system(USER_APP "config/models_update.sh");
    int status;

    GtkApplication *app = gtk_application_new("s21.threedviewer", G_APPLICATION_REPLACE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
