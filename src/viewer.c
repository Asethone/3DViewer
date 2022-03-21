#include "viewer.h"

static render_data_t *render_data = NULL;
extern GtkWindow *window;
extern GtkDrawingArea *area;
extern GtkBuilder *build;
extern settings_t options;
extern bool isScreenshotTime;
extern bool isScreencastTime;
extern int screencastTimer;

static int newModel = 0;
static double SCALE_F = 50;

static void modelInfoUpdate();
static void settingsApply();

void viewModel(const char *filename) {
    obj_data_t *data = g_new0(obj_data_t, 1);
    data->vertices = (double **)malloc(sizeof(double **));
    data->edges = (int **)malloc(sizeof(int **));
    int err = readFile(filename, data);
    if (!err) {
        newModel = 1;
        render_data = newRenderData(data);
        renderModel(render_data);
        modelInfoUpdate();
        gtk_widget_queue_draw(GTK_WIDGET(area));
    } else {
        free(data->vertices);
        free(data->edges);
        errorMessage(err);
    }
}

void viewTransformModel(double *data, transform_type_t type) {
    if (render_data) {
        switch (type) {
            case TRANSLATE:
                translateModel(render_data->obj_data, data);
                break;
            case ROTATE:
                rotateXModel(render_data->obj_data, data[0]);
                rotateYModel(render_data->obj_data, data[1]);
                rotateZModel(render_data->obj_data, data[2]);
                break;
            case SCALE:
                scaleModel(render_data->obj_data, *data);
                break;
        }
        renderModel(render_data);
        gtk_widget_queue_draw(GTK_WIDGET(area));
    } else {
        errorMessage(NO_MODEL);
    }
}

static void applyLineColor(cairo_t *cr) {
    switch (options.e_color) {
        case WHITE:
            cairo_set_source_rgb(cr, 1, 1, 1);
            break;
        case CYAN:
            cairo_set_source_rgb(cr, 0, 1, 1);
            break;
        case GOLDEN:
            cairo_set_source_rgb(cr, 1, 0.87451, 0);
            break;
    }
}

static void applyLineDash(cairo_t *cr) {
    if (options.e_type == DASHED) {
        static const double dashed[] = {5.0};
        cairo_set_dash(cr, dashed, 1, 0);
    }
}

static void drawVertices(cairo_t *cr) {
    if (options.v_shape != NONE) {
        switch (options.v_color) {
            case WHITE:
                cairo_set_source_rgb(cr, 1, 1, 1);
                break;
            case CYAN:
                cairo_set_source_rgb(cr, 0, 1, 1);
                break;
            case GOLDEN:
                cairo_set_source_rgb(cr, 1, 0.87451, 0);
                break;
        }
        cairo_set_line_width(cr, 1);
        for (int i = 0; i < render_data->v_cnt; i++) {
            double v_x = render_data->vertices[i][0] * SCALE_F;
            double v_y = -render_data->vertices[i][1] * SCALE_F;
            if (options.v_shape == CIRCLE) {
                cairo_arc(cr, v_x, v_y, options.v_size, 0, 2 * M_PI);
            } else {
                cairo_rectangle(cr, v_x - options.v_size / 2, v_y - options.v_size / 2, options.v_size,
                                options.v_size);
            }
            cairo_stroke_preserve(cr);
            cairo_fill(cr);
        }
    }
}

static void applyBackgroundColor(cairo_t *cr) {
    switch (options.bg_color) {
        case DARK_GREY:
            cairo_set_source_rgb(cr, 0.13, 0.13, 0.14);
            break;
        case LIGHT_GREY:
            cairo_set_source_rgb(cr, 0.52941, 0.49804, 0.4902);
            break;
        case BLUE:
            cairo_set_source_rgb(cr, 0.11373, 0.16078, 0.31765);
            break;
    }
    cairo_paint(cr);
}

void draw_function(GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer user_data) {
    if (render_data) {
        if (newModel) SCALE_F = 50;
        applyBackgroundColor(cr);
        cairo_set_line_width(cr, 0.25 * options.e_thickness);
        applyLineColor(cr);
        applyLineDash(cr);
        cairo_translate(cr, width / 2, height / 2);
        for (int i = 0; i < render_data->obj_data->e_cnt; i++) {
            for (int j = 0; render_data->obj_data->edges[i][j] != 0; j++) {
                int start = render_data->obj_data->edges[i][j];
                int end = render_data->obj_data->edges[i][j + 1] != 0
                              ? render_data->obj_data->edges[i][j + 1]
                              : render_data->obj_data->edges[i][0];
                double start_x = render_data->vertices[start - 1][0] * SCALE_F;
                double start_y = -render_data->vertices[start - 1][1] * SCALE_F;
                double end_x = render_data->vertices[end - 1][0] * SCALE_F;
                double end_y = -render_data->vertices[end - 1][1] * SCALE_F;
                if (!newModel || ((start_x >= -width / 2 && start_x <= width / 2) &&
                                  (start_y >= -height / 2 && start_y <= height / 2) &&
                                  (end_x >= -width / 2 && end_x <= width / 2) &&
                                  (end_y >= -height / 2 && end_y <= height / 2))) {
                    cairo_move_to(cr, start_x, start_y);
                    cairo_line_to(cr, end_x, end_y);
                    cairo_stroke(cr);
                } else {
                    SCALE_F /= 1.1;
                    applyBackgroundColor(cr);
                    applyLineColor(cr);
                    i = 0;
                    break;
                }
            }
        }
        drawVertices(cr);
        newModel = 0;
    }
    if (isScreenshotTime) {
        cairo_surface_t *surface = cairo_get_target(cr);
        cairo_surface_write_to_png(surface, USER_APP "screenshots/pngay.jpeg");
        cairo_surface_write_to_png(surface, USER_APP "screenshots/pngay.bmp");
        isScreenshotTime = false;
    } else if (isScreencastTime) {
        cairo_surface_t *surface = cairo_get_target(cr);
        char name[255] = "";
        snprintf(name, sizeof(name), USER_APP "screencasts/%02d.png", screencastTimer);
        cairo_surface_write_to_png(surface, name);
        isScreencastTime = false;
    }
}

static void modelInfoUpdate() {
    char text[100] = "";
    GtkWidget *cbox = GTK_WIDGET(gtk_builder_get_object(build, "hbox_file_choose"));
    const char *filename = gtk_entry_buffer_get_text(
        gtk_entry_get_buffer(GTK_ENTRY(gtk_widget_get_first_child(gtk_widget_get_first_child(cbox)))));
    GtkLabel *label = GTK_LABEL(gtk_builder_get_object(build, "label_model"));
    snprintf(text, sizeof(text), "Model: %s", filename);
    gtk_label_set_text(label, text);

    label = GTK_LABEL(gtk_builder_get_object(build, "label_vertices"));
    snprintf(text, sizeof(text), "Number of vertices: %d", render_data->obj_data->v_cnt);
    gtk_label_set_text(label, text);

    label = GTK_LABEL(gtk_builder_get_object(build, "label_edges"));
    snprintf(text, sizeof(text), "Number of edges: %d", render_data->obj_data->e_cnt);
    gtk_label_set_text(label, text);
}

void freeData() {
    if (render_data) {
        // OBJECT DATA
        for (int i = 0; i < render_data->obj_data->v_cnt; i++) {
            free(render_data->obj_data->vertices[i]);
        }
        free(render_data->obj_data->vertices);
        for (int i = 0; i < render_data->obj_data->e_cnt; i++) {
            free(render_data->obj_data->edges[i]);
        }
        free(render_data->obj_data->edges);
        // RENDER DATA
        for (int i = 0; i < render_data->v_cnt; i++) {
            free(render_data->vertices[i]);
        }
        free(render_data->vertices);
        render_data = NULL;
    }
}

void closeDialog(GtkWidget *widget, gpointer user_data) { gtk_window_destroy(GTK_WINDOW(widget)); }

void errorMessage(int err_code) {
    static const char errlist[][100] = ERR_LIST;
    GtkWidget *dialog =
        gtk_message_dialog_new(window, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "Error");
    gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), errlist[err_code - 1]);
    g_signal_connect(dialog, "response", G_CALLBACK(closeDialog), NULL);
    gtk_widget_show(dialog);
}
