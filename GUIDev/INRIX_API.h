#include "Http/lw_http.hpp"
#include <crypto.h>

struct GridData {
    cv::Mat img; 
    std::string matrix_str;
};
bool fetching_data = false;
cv::Mat stitched;
int session = 0;
namespace INRIX {
    using namespace cv;
	std::wstring get_token()
	{
        c_lw_http http;
        c_lw_httpd http_req;
        http.open_session();
        std::string h_reply;
        http.get(L"https://api.iq.inrix.com/auth/v1/appToken?appId=kokjvu5np7&hashToken=a29ranZ1NW5wN3xVbm12elhrOGx5bmliVkU3YklUOTVOZGs2aWNPbERXNzZWQjljdVhm", h_reply);
        json res = json::parse(h_reply);
        std::string token = res["result"]["token"];
        return std::wstring(token.begin(), token.end());
	}
    

    //void stitch_image(std::vector<GridData>& images)
    //{
    //    stitched = Mat(256 * 4, 256 * 4, CV_8UC3);
    //    for (auto& path : images)
    //    {
    //        Mat map = path.img;
    //        std::string name = path.matrix_str;
    //        int pos1 = name[0] - 48;
    //        int pos2 = name[1] - 48;
    //        std::cout << pos1 << " " << pos2 << '\n';
    //        Mat inset(stitched, Rect((256 * ((pos2) % 2)) + (512 * (pos1 % 2)), 256 * ((pos2) / 2) + (512 * (pos1 / 2)), 256, 256));
    //        map.copyTo(inset);
    //    }
    //    /*imshow("stitched", stitched);
    //    waitKey();*/
    //}

    void get_images(std::string QUADKEY,std::string FRCLevel,std::string UTC_DATE,int opacity, int penWidth)
    {
        c_lw_http http;
        session++;
        c_lw_httpd http_req;
        Mat stitched_temp(256 * 4, 256 * 4, CV_8UC3);
        for (int i = 0; i <= 3; i++)
        {
            for (int j = 0; j <= 3; j++)
            {
                char c1 = i + 48;
                char c2 = j + 48;
                auto c3 = std::string(1, c1) + c2;
                std::string quadkey_tmep = QUADKEY+ c3;
                std::wstring frclevel_wstr = std::wstring(FRCLevel.begin(), FRCLevel.end());
                std::wstring quadkey_wstr = std::wstring(quadkey_tmep.begin(), quadkey_tmep.end());
                std::wstring utc_wstr = std::wstring(UTC_DATE.begin(), UTC_DATE.end());

                http.open_session();
                std::string h_reply;
                std::vector<std::wstring> headers = { L"Authorization: Bearer " + get_token() };
                
                std::wstring params = L"?opacity="+ std::to_wstring(opacity) +L"&penWidth="+ std::to_wstring(penWidth) + (FRCLevel=="all"?L"":L"&FRCLevel="+frclevel_wstr) + (UTC_DATE.find_first_not_of("0123456789")==-1? L"&startTime="+utc_wstr : L"");
                http.get(L"https://api.iq.inrix.com/v1/tiles/"+ quadkey_wstr + params, h_reply, headers);
                std::cout << "Fetched QUADKEY: " << quadkey_tmep << "Level: " << FRCLevel << " ...\n";
                std::vector<BYTE> vectordata;
                vectordata.resize(h_reply.size());
                new CryptoPP::StringSource(h_reply, true, new CryptoPP::ArraySink(&vectordata[0], vectordata.size()));
                cv::Mat data_mat(vectordata, true);
                cv::Mat frame = imdecode(vectordata, cv::IMREAD_COLOR); //PGM image
                int pos1 = i;
                int pos2 = j;
                Mat inset(stitched_temp, Rect((256 * ((pos2) % 2)) + (512 * (pos1 % 2)), 256 * ((pos2) / 2) + (512 * (pos1 / 2)), 256, 256));
                frame.copyTo(inset);
                imshow(std::to_string(session)+" QUADKEY " + QUADKEY, stitched_temp);
                waitKey(10);
            }
        }
        stitched = stitched_temp;
        


    }

    
}