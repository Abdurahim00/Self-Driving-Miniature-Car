/*
 * Copyright (C) 2020  Christian Berger
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PROCESS_IMAGE_HPP
#define PROCESS_IMAGE_HPP

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath>

double steeringAngle = 0;
int blueCount = 0;
bool clockWise = false;

std::vector<std::vector<cv::Point>> findContours(const cv::Mat &mask)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    return contours;
}

double processContours(const cv::Mat &result, const std::vector<std::vector<cv::Point>> &contours, double &angle, int yOffset, const cv::Scalar &color)
{
    for (const auto &contour : contours)
    {
        cv::Rect rectangleBorder = cv::boundingRect(contour);
        if (rectangleBorder.area() > 100)
        { // Ensuring that small noises are disregarded
            rectangleBorder.y += yOffset;
            cv::rectangle(result, rectangleBorder, color, 3); // Use the passed color
            int midpointX = rectangleBorder.x + rectangleBorder.width / 2;
            int midpointY = rectangleBorder.y + rectangleBorder.height / 2;
            angle = atan2(midpointY, midpointX);
        }
    }
    return angle;
}


cv::Mat processImage(const cv::Mat &result)
{

    cv::Mat blueMask;
    cv::Mat yellowMask;

    int height = result.rows;
    int croppedTopHeight = height * 0.4; // 40% from the tops
    int remainingHeight = height * 0.5;  // Middle 50% of the height

    // Define the region of interest to exclude the bottom 5%
    cv::Rect roi(0, croppedTopHeight, result.cols, remainingHeight);

    // Create a Mat that only includes the middle 50% of the original image (excluding top 40% and bottom 5%)
    cv::Mat imgBottom60 = result(roi);

    auto convertToHSV = [](const cv::Mat &input)
    {
        cv::Mat HueSatVal;
        cv::cvtColor(input, HueSatVal, cv::COLOR_BGR2HSV);
        return HueSatVal;
    };

    auto createMask = [](const cv::Mat &HueSatVal, const cv::Scalar &minRange, const cv::Scalar &maxRange)
    {
        cv::Mat mask;
        cv::inRange(HueSatVal, minRange, maxRange, mask);
        return mask;
    };

    auto applyMorphologicalOpening = [](cv::Mat &mask)
    {
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    };

    auto applyGaussianBlur = [](cv::Mat &mask)
    {
        cv::GaussianBlur(mask, mask, cv::Size(3, 3), 1.0);
    };

    auto combineMasks = [](const cv::Mat &mask1, const cv::Mat &mask2)
    {
        cv::Mat combinedMask;
        cv::bitwise_or(mask1, mask2, combinedMask);
        return combinedMask;
    };

    // Apply lambda functions values
    cv::Mat hsvImage = convertToHSV(imgBottom60);

    blueMask = createMask(hsvImage, cv::Scalar(99, 118, 41), cv::Scalar(139, 255, 255));
    applyMorphologicalOpening(blueMask);
    applyGaussianBlur(blueMask);

    yellowMask = createMask(hsvImage, cv::Scalar(19, 101, 99), cv::Scalar(29, 255, 255));
    applyMorphologicalOpening(yellowMask);
    applyGaussianBlur(yellowMask);

    cv::Mat combinedMask = combineMasks(blueMask, yellowMask);
    std::vector<std::vector<cv::Point>> blueContours = findContours(blueMask);
    std::vector<std::vector<cv::Point>> yellowContours = findContours(yellowMask);

    double blueAngle = 0.0;
    double yellowAngle = 0.0;

    cv::Scalar blueColor(255, 0, 0);
    cv::Scalar yellowColor(0, 255, 255);

    int yOffset = height * 0.4;

    blueAngle = processContours(result, blueContours, blueAngle, yOffset, blueColor);
    yellowAngle = processContours(result, yellowContours, yellowAngle, yOffset, yellowColor);


    // Calculate midpoint of the screen
    int midX = result.cols / 2;
    int midY = result.rows / 2;

    double distBlue = std::numeric_limits<double>::max();
    double distYellow = std::numeric_limits<double>::max();

    bool bluePresent = false;
    bool yellowPresent = false;

    int blueX = 0, yellowX = 0;

    // Check if blue cone is visible and calculate its distance to midpoint
    if (!blueContours.empty() && !blueContours[0].empty())
    {
        cv::Point bluePoint = blueContours[0][0];
        distBlue = std::sqrt(std::pow(bluePoint.x - midX, 2) + std::pow(bluePoint.y - midY, 2));
        blueX = bluePoint.x;
        bluePresent = true;
    }

    // Check if yellow cone is visible and calculate its distance to midpoint
    if (!yellowContours.empty() && !yellowContours[0].empty())
    {
        cv::Point yellowPoint = yellowContours[0][0];
        distYellow = std::sqrt(std::pow(yellowPoint.x - midX, 2) + std::pow(yellowPoint.y - midY, 2));
        yellowX = yellowPoint.x;
        yellowPresent = true;
    }

    if (bluePresent && yellowPresent)
    {
        blueCount = 0;

        if (blueX > midX && yellowX < midX) // ccw

        {
            clockWise = false;
            if (abs(midX - yellowX) > abs(blueX - midX))
            {
                steeringAngle = (std::abs((midX - yellowX) - (blueX - midX)) * 0.12) / midX;
            }
            else
            {
                steeringAngle = (std::abs((midX - yellowX) - (blueX - midX)) * -0.12) / midX;
            }
        }
        else if (blueX < midX && yellowX > midX) // cw

        {
            clockWise = true;
            if (abs(midX - blueX) > abs(yellowX - midX))
            {
                // Blue cone is on the left side, and yellow cone is on the right side
                steeringAngle = (std::abs((midX - blueX) - (yellowX - midX)) * -0.12) / midX;
            }
            else
            {
                steeringAngle = (std::abs((midX - blueX) - (yellowX - midX)) * 0.12) / midX;
            }
        }
        cv::putText(result, "BlueX: " + std::to_string(blueX) + "YellowX: " + std::to_string(yellowX) + "GS" + std::to_string(steeringAngle), cv::Point(150, 80),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1, cv::LINE_AA);
    }

    else if (bluePresent && !yellowPresent)
    {
        blueCount++;
        // Only blue cone is present
        if (clockWise)
        {
            if (blueCount > 30)
            {
                steeringAngle = -0.15;
            }
            else
            {
                // Blue cone is on the left side
                steeringAngle = -0.1;
            }
        }
        else if (!clockWise)
        {
            // Blue cone is on the right side
            steeringAngle = 0.1;
        }
    }
    else if (yellowPresent && !bluePresent)
    {
        // Only yellow cone is present
        if (!clockWise)
        {
            // Yellow cone is on the left side
            steeringAngle = 0.1;
        }
        else if (clockWise)
        {
            // Yellow cone is on the right side
            steeringAngle = -0.1;
        }
    }

    return result;
}

#endif

int32_t main(int32_t argc, char **argv)
{
    std::ofstream dataFile("/tmp/output.txt", std::ios::trunc);
    if (!dataFile)
    {
        std::cerr << "Unable to open file for output." << std::endl;
        return -1; // Exit if file cannot be opened
    }
    dataFile << "group_06;sampleTimeStamp;steeringWheelAngle\n";

    int32_t retCode{1};
    // Parse the command line parameters as we require the user to specify some mandatory information on startup.
    auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
    if ((0 == commandlineArguments.count("cid")) ||
        (0 == commandlineArguments.count("name")) ||
        (0 == commandlineArguments.count("width")) ||
        (0 == commandlineArguments.count("height")))
    {
        std::cerr << argv[0] << " attaches to a shared memory area containing an ARGB image." << std::endl;
        std::cerr << "Usage:   " << argv[0] << " --cid=<OD4 session> --name=<name of shared memory area> [--verbose]" << std::endl;
        std::cerr << "         --cid:    CID of the OD4Session to send and receive messages" << std::endl;
        std::cerr << "         --name:   name of the shared memory area to attach" << std::endl;
        std::cerr << "         --width:  width of the frame" << std::endl;
        std::cerr << "         --height: height of the frame" << std::endl;
        std::cerr << "Example: " << argv[0] << " --cid=253 --name=img --width=640 --height=480 --verbose" << std::endl;
    }
    else
    {
        // Extract the values from the command line parameters
        const std::string NAME{commandlineArguments["name"]};
        const uint32_t WIDTH{static_cast<uint32_t>(std::stoi(commandlineArguments["width"]))};
        const uint32_t HEIGHT{static_cast<uint32_t>(std::stoi(commandlineArguments["height"]))};
        const bool VERBOSE{commandlineArguments.count("verbose") != 0};

        // Attach to the shared memory.
        std::unique_ptr<cluon::SharedMemory> sharedMemory{new cluon::SharedMemory{NAME}};
        if (sharedMemory && sharedMemory->valid())
        {
            std::clog << argv[0] << ": Attached to shared memory '" << sharedMemory->name() << " (" << sharedMemory->size() << " bytes)." << std::endl;

            // Interface to a running OpenDaVINCI session where network messages are exchanged.
            // The instance od4 allows you to send and receive messages.
            cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"]))}; 

            //od4.dataTrigger(opendlv::proxy::AngularVelocityReading::ID(), onAngularVelocityRequest);
            // Endless loop; end the program by pressing Ctrl-C.
            while (od4.isRunning())
            {
                // OpenCV data structure to hold an image.
                cv::Mat img;

                // Wait for a notification of a new frame.
                sharedMemory->wait();

                // Lock the shared memory.
                sharedMemory->lock();
                {
                    // Copy the pixels from the shared memory into our own data structure.
                    cv::Mat wrapped(HEIGHT, WIDTH, CV_8UC4, sharedMemory->data());
                    img = wrapped.clone();
                }

                // TODO: Here, you can add some code to check the sampleTimePoint when the current frame was captured.
                int64_t sampleTimePointInMicroseconds = 0;
                auto timeStampResult = sharedMemory->getTimeStamp();
                if (timeStampResult.first)
                { // Check if getTimeStamp was successful
                    cluon::data::TimeStamp sampleTimePoint = timeStampResult.second;
                    // Convert TimeStamp to microseconds
                    sampleTimePointInMicroseconds = cluon::time::toMicroseconds(sampleTimePoint);
                    // Now you have the time point in microseconds, which you can use as needed
                    // For example, logging or performing time-based calculations
                    //std::cout << "Sample Time Point (microseconds): " << sampleTimePointInMicroseconds << std::endl;

                }

                sharedMemory->unlock();

                // Get current time
                auto now = std::chrono::system_clock::now();
                auto in_time_t = std::chrono::system_clock::to_time_t(now);

                // Convert time to string form
                std::stringstream ss;
                ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X"); // Format: YYYY-MM-DD HH:MM:SS
                std::string dateAndTime = ss.str();

                std::string GroupName = "Group 6";
                std::string textToOverlay = "Now " + dateAndTime + "; " + "ts: " + std::to_string(sampleTimePointInMicroseconds) + "; " + GroupName;
                // TODO: Do something with the frame.
               
                cv::Mat processedImage = processImage(img);

                dataFile << "group_06;" << sampleTimePointInMicroseconds << ";" << steeringAngle << std::endl;
                std::cout << "group_06;" << sampleTimePointInMicroseconds << ";" << steeringAngle << std::endl;



                int textPositionX = 10; // X coordinate
                int textPositionY = img.rows - 10;
                cv::putText(img, textToOverlay, cv::Point(textPositionX, textPositionY),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);

                // Display image on your screen.
                if (VERBOSE)
                {
                    cv::imshow(sharedMemory->name().c_str(), img);
                    cv::waitKey(1);
                }
            }
        }
        std::cout << "Closing the file now." << std::endl;
        dataFile.close();
        retCode = 0;
    }
    return retCode;
}