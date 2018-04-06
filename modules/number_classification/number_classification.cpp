#include "number_classification.h"

NumberClassification::NumberClassification() {

}

int NumberClassification::Classify(const Ref<Image> &p_image, Rect2 rect) {
  Ref<Image> img = p_image->duplicate();
  int width = img->get_width();
  int height = img->get_height();
  print_line(itos(height));
  print_line(itos(width));
  print_line(itos(p_image->get_format()));
  if (img->is_compressed()) {
    print_line("Is compressed");
    img->decompress();
  }

  if (img->detect_alpha()) {
    print_line("Has alpha");
  }
  img->convert(Image::FORMAT_RGBA8);
  img->flip_y();
  PoolVector<uint8_t> data = img->get_data();
  PoolVector<uint8_t>::Read r = data.read();
  cv::Mat image = cv::Mat(height, width, CV_8UC4, const_cast<unsigned char*>(r.ptr())).clone();
  cv::Rect roi(rect.position.x, rect.position.y,
               rect.size.width, rect.size.height);
  cv::Mat clipped_image(image, roi);
  /* cv::Mat reshapedImage = imageWithData.reshape(0, height); */
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Display window", clipped_image);








  /* cv::Mat image = imread("/home/niklas/Projects/Interactive-Visualization-Tool-Core/img.jpg", cv::IMREAD_COLOR); */
  /* if (image.empty()) { */
  /*   print_line("Could not load image"); */
  /*   return -1; */
  /* } */
  /* cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE ); // Create a window for display. */
  /* cv::imshow( "Display window", image ); */
  cv::waitKey(0);
  return 1;
}

void NumberClassification::_bind_methods() {
  ClassDB::bind_method(D_METHOD("Classify", "img", "rect"), &NumberClassification::Classify);
}
