#include <sstream>
#include <iostream>
#include <fstream>
//#include <tchar.h>


#include <limits>
#include <string>
#include <time.h>

#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"






inline std::vector<std::string> _splitString(char* str, const char* seps)
{
	std::vector<std::string> sub_strs;
	char* token = strtok(str, seps);
	while (token) {
		std::string sub(token);
		sub = sub.substr(0, sub.find_last_of("\t\r\n"));
		sub_strs.push_back(sub);
		token = strtok(NULL, seps);
	}
	return sub_strs;
}

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char * argv[])
//int main(int argc, char* argv[]);
{
	auto start_time = clock();
	if (argc != 2) {
		printf("input a .tsk");
		return 1;
	}
	std::string lbuf;
	std::string tsk_path =argv[1];
	std::ifstream fin;
	fin.open(tsk_path.c_str());
	if (!fin.is_open()) {
		std::cout << "failed to open file: " << tsk_path << std::endl;
		return 1;
	}
	char img_path[1024], output_path[1024];
	float m_length(-1);
	getline(fin, lbuf);
	char sbuf[4096]; 
	strcpy(sbuf, lbuf.c_str());
	std::vector<std::string> seps = _splitString(sbuf, " ");
	std::cout<<seps.size()<<std::endl;
	if (seps.size() < 2) {
		fin.close(); return 1;		
	}

	strcpy(img_path, seps[0].c_str());
	strcpy(output_path, seps[1].c_str());
	
	if (seps.size() >= 6 && !seps[5].empty()) {
		sscanf(seps[5].c_str(), "%f", &m_length);
	}
	
	fin.close();

	cv::Mat img_mat = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
	if (img_mat.empty()) {
		std::cout << "error: cannot load image: " << img_path << std::endl;
		return 1;
	}

	cv::blur(img_mat, img_mat, cv::Size(3, 3));
	cv::Ptr<cv::LineSegmentDetector> ls = cv::createLineSegmentDetector(cv::LSD_REFINE_STD);
	std::vector<cv::Vec4f> detected_lines;	///< (0,1) - the start point; (2,3) - the end point
	ls->detect(img_mat, detected_lines);
	
	FILE* fp = fopen(output_path, "w");
	if (!fp) {
		std::cout << "failed to open file: " << output_path << std::endl;
		return 1;
	}

	int index(0);

	float leng_thre;
	bool bLen = m_length > 0;
	if (bLen) {
		leng_thre = m_length * m_length;
	}
	for (auto& cv_line : detected_lines) {

		if (bLen) {
			if (pow(cv_line[0]-cv_line[2],2) +pow(cv_line[1]- cv_line[3],2) < leng_thre) {
				continue;
			}
		}
		fprintf(fp, "%10d %12.4lf %12.4lf %12.4lf %12.4lf\n", index, 
			cv_line[0], 
			img_mat.rows - cv_line[1],
			cv_line[2],
			img_mat.rows - cv_line[3]);
		index++;
	}
	
	fclose(fp);

	auto end_time = clock();
	std::cout << index  << "lines found from " << img_path << " cost " << (end_time - start_time) / (double)CLOCKS_PER_SEC << " s" << std::endl;
	return 0;
}
