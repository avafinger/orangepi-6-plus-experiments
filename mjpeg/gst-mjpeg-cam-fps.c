#include <gst/gst.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

double __get_us(struct timeval t) { return (t.tv_sec * 1000000 + t.tv_usec); }

int cam_width, cam_height;
char *cam_video;
int skip_parsing = 0;

int main(int argc, char *argv[])
{
    if (argc != 5) {
       g_printerr("Missing parameter: videoX (ex: /dev/video6) 1920 (frame width) 1080 (frame height) 0 (or 1 = skip parsing for some old MJPEG cameras)\n");
       return -1;
    }
    cam_video = argv[1];
    cam_width = atoi(argv[2]);
    cam_height = atoi(argv[3]);
    skip_parsing = atoi(argv[4]);

    gst_init(&argc, &argv);

    // Create GStreamer pipeline
    GstElement *pipeline = gst_pipeline_new("video-pipeline");
    // Create elements
    GstElement *source = gst_element_factory_make("v4l2src", "source");
    // GstElement *depay = gst_element_factory_make("rtph264depay", "depay");
    GstElement *source_capsfilter = gst_element_factory_make("capsfilter", "v4l2_capsfilter");
    GstElement *parser = gst_element_factory_make("jpegparse", "jpeg-parser");
    GstElement *decoder = gst_element_factory_make("v4l2jpegdec", "jpeg-decoder");
    /* available video sinks: "kmssink","glimagesink","waylandsink" */
    GstElement *fps_sink = gst_element_factory_make("fpsdisplaysink", "fps-sink");
    GstElement *sink = gst_element_factory_make("glimagesink", "real-sink");
    if (!pipeline || !source || !source_capsfilter || !parser || !decoder || !fps_sink || !sink) {
        g_printerr("One element could not be created. Exiting.\n");
        return -1;
    }

    /* Set input video file for source element */
    g_object_set(G_OBJECT(source), "device", cam_video, NULL);
    g_object_set(G_OBJECT(source), "io-mode", 4, NULL);
    // g_object_set(G_OBJECT(source), "num-buffers", 1000, NULL);

    GstCaps *caps = gst_caps_new_simple("image/jpeg",
                                        "format", G_TYPE_STRING, "MJPG",
                                        "width", G_TYPE_INT, cam_width,
                                        "height", G_TYPE_INT, cam_height,
                                        NULL);
    g_object_set(G_OBJECT(source_capsfilter), "caps", caps, NULL);
    gst_caps_unref(caps);

    g_object_set(G_OBJECT(fps_sink),
                 "video-sink", sink,
                 "text-overlay", TRUE,
                 "signal-fps-measurements", TRUE,
                 NULL);
    g_object_set(G_OBJECT(sink), "sync", FALSE, NULL);

    if (skip_parsing) {
       gst_bin_add_many(GST_BIN(pipeline), source, source_capsfilter, decoder, fps_sink, NULL);
       if (!gst_element_link_many(source, source_capsfilter, decoder, fps_sink, NULL)) {
           g_printerr("Elements sink could not be linked.\n");
           gst_object_unref(pipeline);
           return -1;
       }
    } else {
      gst_bin_add_many(GST_BIN(pipeline), source, source_capsfilter, parser, decoder, fps_sink, NULL);
      if (!gst_element_link_many(source, source_capsfilter, parser, decoder, fps_sink, NULL)) {
          g_printerr("Elements sink could not be linked.\n");
          gst_object_unref(pipeline);
          return -1;
      }
    }

    // Start playing the pipeline
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    // Run the main loop
    GMainLoop *main_loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(main_loop);

    // Clean up
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(sink);
    gst_object_unref(fps_sink);
    gst_object_unref(decoder);
    gst_object_unref(parser);
    gst_object_unref(source);
    gst_object_unref(pipeline);
    g_main_loop_unref(main_loop);

    return 0;
}
