#pragma once
#include<string>
#include<opencv2/opencv.hpp>
#include "FaceComponent.h"
#include "FaceAnalysisModel.h"
#include "Face.h"
#include "ResourceManagers.h"
#include"FrameSequenceProcessor.h"
class FatigueDetectionFrameSequenceProcessor
        :public FrameSequenceProcessor {
private:
	struct EyeParameters {
		std::string eyeStatusStr_ = "EYE STATUS:";
		cv::Scalar eyeStatusColor_=cv::Scalar(0,255,0);
		unsigned int openEyeFrameCount_ = 0;
		unsigned int closeEyeFrameCount_ = 0;
		float perclos_=0;
		cv::Scalar perclosColor_ = cv::Scalar(0,255,0);
        const float perclosThreshold_=0.75;
		const int eyeJudgingInterval_ = 3;
      void  reset(){
            eyeStatusStr_= "EYE STATUS:";
            eyeStatusColor_=cv::Scalar(0,255,0);
           openEyeFrameCount_=0;
           closeEyeFrameCount_=0;
           perclos_=0;
           perclosColor_=cv::Scalar(0,255,0);
        }
	};
	struct MouthParameters {
		std::string mouthStatusStr_ = "MOUTH STATUS:";
		cv::Scalar mouthStatusColor_=cv::Scalar(0,255,0);
		unsigned int openMouthFrameCount_ = 0;
		unsigned int closeMouthFrameCount_ = 0;
		const int mouthJudgingInterval = 30;
       void reset(){
            mouthStatusStr_= "MOUTH STATUS:";
            mouthStatusColor_=cv::Scalar(0,255,0);
            openMouthFrameCount_=0;
            closeMouthFrameCount_=0;
        }
	};
	struct YawnParameters {
		time_t startOpenMouthTime_=0;
		unsigned int currentOpenMouthFrameCount_ = 0;
		unsigned int interuptCloseMouthFrameCount_ = 0;
		FaceComponent::Status lastMouthStatus_ = FaceComponent::Status::close;
		unsigned int yawnFrameCount_ = 0;
		unsigned int yawnCount_ = 0;
		float yawnFrameRate_ = 0;
		cv::Scalar yawnFrameRateColor_ = cv::Scalar(0,255,0);
		const int  yawnContinueTimeThreshold_ = 2;
		const float yawnFrameRateThreshold_ = 0.2;
        void reset(){
            startOpenMouthTime_=0;
            currentOpenMouthFrameCount_=0;
            interuptCloseMouthFrameCount_=0;
            lastMouthStatus_=FaceComponent::Status::open;
            yawnFrameCount_=0;
            yawnCount_=0;
            yawnFrameRate_=0;
            yawnFrameRateColor_=cv::Scalar(0,255,0);
        }
	};
	struct SystemParameters {
		time_t nowTime_ = 0;
		time_t lastSecond_=0;
		unsigned int rawFrameCount_=0;
		unsigned int rawFrameCountAtLastMinute_=0;
		unsigned int aveRawFPSInOneMinute_ = 0;
		unsigned int faceFrameCount_=0;
		unsigned int faceFrameCountAtLastSecond_=0;
		unsigned int fps_=0;
		unsigned int fatigueFrameCount_ = 0;
        void reset(){
            nowTime_=0;
            lastSecond_=0;
            rawFrameCount_=0;
            rawFrameCountAtLastMinute_=0;
            aveRawFPSInOneMinute_=0;
            faceFrameCount_=0;
            faceFrameCountAtLastSecond_=0;
            fps_=0;
            fatigueFrameCount_=0;
        }
	};
    struct FaceParameters{
        std::string directionStr_="Front";
        cv::Scalar color_=cv::Scalar(0,255,0);
        unsigned int leftFaceFrameCount_=0;
        unsigned int rightFaceFrameCount_=0;
        unsigned int frontFaceFrameCount_=0;
        FaceAnalysisModel::FaceType lastFaceDirection_=FaceAnalysisModel::FaceType::Front;
        time_t startDistractionTime_=0;
        unsigned int interuptDistractionFrameCount_=0;
        const int distractionDetectionInterval=5;
        void reset(){
            directionStr_="Front";
            color_=cv::Scalar(0,255,0);
            leftFaceFrameCount_=0;
            rightFaceFrameCount_=0;
            frontFaceFrameCount_=0;
            lastFaceDirection_=FaceAnalysisModel::FaceType::Front;
            startDistractionTime_=0;
            interuptDistractionFrameCount_=0;
        }
    };
    EyeParameters rightEyeParam_,leftEyeParam_,eyesParam_;
	MouthParameters mouthParam_;
	YawnParameters yawnParam_;
	SystemParameters systemParam_;
    FaceParameters faceParam_;

     cv::Ptr<FaceAnalysisModel> pFaceAnalysisModel_;
	Face face_;
    //evvm:eye evidence video manager,mevm_:mouth evidence video manager
    VideoManager vm_, eevm_, mevm_;
	std::string videoFolder_ = "";
    std::string eeFolder_="";
    std::string meFolder_="";
public:
    FatigueDetectionFrameSequenceProcessor(const cv::Ptr<FaceAnalysisModel> & pfam
                           , const std::string &videoFolder
                           , const std::string &eeFolder
                           , const std::string &meFolder);
    ~FatigueDetectionFrameSequenceProcessor();
    void beforeProcess() override;
    void process(cv::Mat rawFrame) override;
    void afterProcess() override;
  /*VideoManager & getVm();
    VideoManager & getEevm();
    VideoManager & getMevm();*/
private:
	void updateEyeParameters(EyeParameters &eyeParam,FaceComponent::Status eyeStatus);
	void updateMouthParameters(MouthParameters &mouthParam,FaceComponent::Status mouthStatus);
    void updateFaceParameters(FaceParameters &faceParam,FaceAnalysisModel::FaceType faceType);
	void countYawnFrame(FaceComponent::Status mouthStatus);
    bool detectDistractionByTimeInterval(FaceAnalysisModel::FaceType,int interval=2);
    bool detectDistractionByFrameRate(float distractionThreshold);
    bool judgeFatigueByEye(EyeParameters &eyeParams);
    bool judgeFatigueByMouth();
	void updateFPS();
    void printParamsToLeft(cv::Mat &colorImg);
    void printParamsToMiddle(cv::Mat &colorImg);
    void printParamsToRight(cv::Mat &colorImg);
	void updateAveRawFPSInOneMinute();
	void resetParameters();
	void checkRawFrameCount();
};
