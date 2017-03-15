//
//  ReplayKitsController.hpp
//  IF
//
//  Created by liruiqi on 15/9/29.
//
//

#ifndef ReplayKitsController_hpp
#define ReplayKitsController_hpp

enum RecordStatus{
    WAIT,
    RECORDING,
};

class ReplayKitsController{
public:
    static ReplayKitsController* getInstance();
    void startRecord();
    void stopRecord();
    void openReplay();
    int getStatus();//0.未开始 1.正在录制
    bool isReplaySupport();
};

#endif /* ReplayKitsController_hpp */
