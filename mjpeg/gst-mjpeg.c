#include <gst/gst.h>

int main(int argc, char *argv[]) 
{
  GstElement *pipeline, *source, *parser, *decoder, *sink, *freeze;
  GstBus *bus;
  GstMessage *msg;

  if (argc != 2) {
     g_printerr("Missing parameter: file_name.jpg\n");
     return -1;
  }

  /* 1. Initialize GStreamer */
  gst_init(&argc, &argv);

  /* 2. Create Elements */
  pipeline = gst_pipeline_new("jpeg-decoder-pipeline");
  source   = gst_element_factory_make("filesrc", "file-source");
  parser   = gst_element_factory_make("jpegparse", "jpeg-parser");
  decoder  = gst_element_factory_make("v4l2jpegdec", "v4l2-decoder");
  freeze   = gst_element_factory_make("imagefreeze", "freeze-output");
  sink     = gst_element_factory_make("glimagesink", "video-output");

  if (!pipeline || !source || !parser || !decoder || !sink || !freeze ) {
    g_printerr("One or more elements could not be created.\n");
    return -1;
  }

  /* 3. Configure Elements (e.g., set input file path) */
  g_object_set(G_OBJECT(source), "location", argv[1], NULL);

  /* 4. Build the Pipeline */
  gst_bin_add_many(GST_BIN(pipeline), source, parser, decoder, freeze, sink, NULL);
  if (!gst_element_link_many(source, parser, decoder, freeze, sink, NULL)) {
    g_printerr("Elements could not be linked.\n");
    gst_object_unref(pipeline);
    return -1;
  }

  /* 5. Start Playback */
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  /* 6. Wait until error or EOS (End of Stream) */
  bus = gst_element_get_bus(pipeline);
  msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* 7. Free Resources */
  if (msg != NULL) gst_message_unref(msg);
  gst_object_unref(bus);
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);

  return 0;
}
