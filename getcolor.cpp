#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

int main() {
    cv::Mat image = cv::imread("/home/user/.wallpaper.jpg");
    if (image.empty()) {
        std::cout << "error when loading" << std::endl;
        return -1;
    }

    // Преобразование в HSV
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    // Разделение на каналы
    std::vector<cv::Mat> channels;
    cv::split(hsvImage, channels);

    // Нахождение доминирующего цвета в канале Hue
    int histSize = 256; // количество бинов
    float range[] = {0, 256};
    const float* histRange = {range};
    cv::Mat hist;
    cv::calcHist(&channels[0], 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false);

    // Находим максимальное значение в гистограмме
    double maxVal = 0;
    cv::minMaxLoc(hist, 0, &maxVal);

    // Переводим доминирующий оттенок в цвет RGB
    int dominantHue = maxVal;
    cv::Mat dominantColor(1, 1, CV_8UC3, cv::Scalar(dominantHue, 255, 255));
    cv::cvtColor(dominantColor, dominantColor, cv::COLOR_HSV2BGR);

    // Выводим доминирующий цвет в формате HEX
    cv::Vec3b rgbColor = dominantColor.at<cv::Vec3b>(0, 0);
    char hexColor[8];
    sprintf(hexColor, "#%02X%02X%02X", rgbColor[2], rgbColor[1], rgbColor[0]);
    std::cout << hexColor << std::endl;

    std::ofstream myfile;
    myfile.open("~/.config/dwm.conf");
    if (myfile.is_open()) {
        myfile << hexColor << std::endl;
        myfile.close();
    } else {
        std::cout << "Unable to open file" << std::endl;
    }
    return 0;
}