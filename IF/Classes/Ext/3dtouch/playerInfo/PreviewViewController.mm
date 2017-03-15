//
//  PreviewViewController.mm
//  IF
//
//  Created by liuzengyou on 13-8-28.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#import "PreviewViewController.h"
#import "TopHeadView.h"
#import "RoleInfoItemOC.h"
#import "RoleInfoViewOC.h"
#import "RoleInfoItemTitleOC.h"

#include "../OCCppHelper.h"
#include "../../../model/GlobalData.h"
#include "../../../controller/AchievementController.h"
#include "../../../controller/GeneralManager.h"
#include "../../../controller/ArmyController.h"
#include "../../../controller/TroopsController.h"
#include "../../../controller/FunBuildController.h"


@interface PreviewViewController ()<TopHeadViewDelegate>
{
    CGFloat angle;
    BOOL _refleshStop;
}
@property (nonatomic,strong) TopHeadView * topHeaderView;
@property (nonatomic,strong) RoleInfoViewOC *roleInfoViewOC;

@property (strong,nonatomic) UIView * headView;
@property (nonatomic,strong)   dispatch_queue_t dispatch_getQueue;

@property (strong,nonatomic) UIView * infoListView;

@property (strong,nonatomic) UIScrollView * scrollView;
@property (strong,nonatomic) UIImageView* refleshView;

@end

@implementation PreviewViewController


 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
        self.view.frame = [[UIScreen mainScreen] bounds];
        
        angle = 0.0f;
        _refleshStop = NO;
        
        self.dispatch_getQueue = dispatch_queue_create("com.playerInfo", NULL);
        
        
    }
    return self;
}


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = [UIColor whiteColor];
    [self.view addSubview:self.headView];
    [self.headView addSubview:self.topHeaderView];
    [self.view addSubview:self.roleInfoViewOC];
    
    [self.view addSubview:self.infoListView];
    [self.infoListView addSubview:self.scrollView];
    
    [[NSNotificationCenter defaultCenter]addObserver:self
                                            selector:@selector(notificationForUpdatePlayerInfoWithNotify:)
                                                name:IOS_kUPDATE_PLAYERINFO
                                              object:nil];
    [self initView];
    
    [self initDetailView];
    
    
    _refleshView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"loading_1.png"]];
    _refleshView.center = CGPointMake(self.view.frame.size.width / 2, self.view.height / 3+10);
    _refleshView.bounds = CGRectMake(0.0f, 0.0f, 30, 30);
    [self.view addSubview:_refleshView];
    
    
//    [self startAnimation];

}

- (void)startAnimation

{
    
    [UIView beginAnimations:nil context:nil];
    
    [UIView setAnimationDuration:0.02];
    
    [UIView setAnimationDelegate:self];
    
    [UIView setAnimationDidStopSelector:@selector(endAnimation)];
    
    _refleshView.transform = CGAffineTransformMakeRotation(angle * (M_PI /180.0f));
    
    [UIView setAnimationRepeatCount: 0];
    
    [UIView commitAnimations];
    
}

-(void)endAnimation
{
    if (_refleshStop==YES)
    {
//         [_refleshView removeFromSuperview];
//        _refleshView = nil;
        return;
    }
    angle += 15;
    
//    [self startAnimation];
    
}

-(UIView *)headView{
    if (_headView == nil) {
        UIView *headView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, kMainScreenWidth, kMainScreenHeight *0.07)];
        headView.backgroundColor = [UIColor grayColor];
        _headView = headView;
    }
    return _headView;
}

-(TopHeadView*)topHeaderView{
    if (_topHeaderView ==nil) {
        TopHeadView *view = [TopHeadView  topHeadViewWithRightButtonShowFlag:NO andShowRightButtonType:TopHeadViewRightButtonType_ViewEdit];
        view.titleNameLabel.text = [NSString stringWithMultilingualWithKey:@"105000"];
        view.titleNameLabel.font = [UIFont systemFontOfSize:13.0];
        view.frame = self.headView.frame;
        view.topHeadViewDelegate = self;
        _topHeaderView = view;
    }
    return _topHeaderView;
}

-(RoleInfoViewOC*)roleInfoViewOC
{
    if (_roleInfoViewOC==nil)
    {
        RoleInfoViewOC *view = [RoleInfoViewOC  showPlayerInfo];
        view.frame =  CGRectMake(0, self.headView.frame.size.height, kMainScreenWidth, 135);
        view.backgroundColor = [UIColor grayColor];
        _roleInfoViewOC = view;
    }
    
    return _roleInfoViewOC;
}

-(UIView *)infoListView{
    if (_infoListView == nil) {
        CGFloat h = self.headView.frame.size.height+self.roleInfoViewOC.frame.size.height;
        UIView *infoListView = [[UIView alloc]initWithFrame:CGRectMake(0, h, kMainScreenWidth, kMainScreenHeight-h)];
        infoListView.backgroundColor = [UIColor grayColor];
        _infoListView = infoListView;
        
    }
    return _infoListView;
}

-(UIView *)scrollView{
    if (_scrollView == nil) {
        
        UIScrollView* view =[[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, self.infoListView.frame.size.width, self.infoListView.frame.size.height)];
//        view.contentSize=CGSizeMake(self.infoListView.frame.size.width,1400);

        view.pagingEnabled=YES;//是否自己动适应
        
        view.backgroundColor = [UIColor colorWithPatternImage:[UIImage imageNamed:@"board_04.png"]];
        
        _scrollView = view;
    }
    return _scrollView;
}

- (void) initDetailView
{
    int endNum=2;
    int area=2;
    int _num = 105010;
    int _hight = 0;
    for(;area<=endNum;area++){
        for (int i=0; i<11; i++) {
            if (i==0) {
                RoleInfoItemTitleOC* _RoleInfoItemTitleOC = [RoleInfoItemTitleOC showPlayerInfoDetailItemTitle];
                
                _RoleInfoItemTitleOC.frame =  CGRectMake(0, _hight, kMainScreenWidth, 30);
                _RoleInfoItemTitleOC.tag = area*10+i;
                
                [self.scrollView addSubview:_RoleInfoItemTitleOC];
                _RoleInfoItemTitleOC.m_title.text = [NSString stringWithMultilingualWithKey:@"105010"];
               
                _hight += 30;
            }else{
                
                RoleInfoItemOC* _RoleInfoItemOC = [RoleInfoItemOC showPlayerInfoDetailItem];
                
                _RoleInfoItemOC.frame =  CGRectMake(0, _hight, kMainScreenWidth, 23.5);
                _RoleInfoItemOC.tag = area*10+i;
                
                [self.scrollView addSubview:_RoleInfoItemOC];
                
                _num += 1;
                
                NSString *name = [NSString stringWithMultilingualWithKey:[NSString stringWithFormat:@"%d", _num]];
                
                _RoleInfoItemOC.m_nameTxt.text = name;
                _RoleInfoItemOC.m_valueTxt.text = @"??????";
                
                int cell_type=i%2;
                _RoleInfoItemOC.m_bg0.hidden = cell_type==0?NO:YES;
                
                _hight += 23.5;
            }
        }
    }
    
    
    self.scrollView.contentSize=CGSizeMake(self.infoListView.frame.size.width,_hight);
}

-(void) initView
{
    [_roleInfoViewOC.m_headImg setImage: [UIImage imageNamed:@"g026.png"]];
    
    NSString* _tmp = [NSString stringWithFormat:@"%@:????", [NSString stringWithMultilingualWithKey:@"102163"]];
    
    _roleInfoViewOC.m_powerTxt.text = _tmp;
 
    _tmp = [NSString stringWithFormat:@"%@:????", [NSString stringWithMultilingualWithKey:@"105001"]];
    
    _roleInfoViewOC.m_killTxt.text = _tmp;

    _tmp = [NSString stringWithFormat:@"%@??%%", [NSString stringWithMultilingualWithKey:@"137409"]];
    _roleInfoViewOC.m_achieveTTF.text = _tmp;
    
    _tmp = [NSString stringWithFormat:@"%@????", [NSString stringWithMultilingualWithKey:@"137410"]];
    _roleInfoViewOC.m_medalTTF.text = _tmp;
    
    std::string leagueShort = std::string("(????)");
    
    string str = leagueShort;
    str.append("????");
    
    _tmp = [NSString stringWithFormat:@"%@", [NSString stringWithUTF8String:str.c_str()]];
    _roleInfoViewOC.m_nameTxt.text = _tmp;
    
    _tmp = [NSString stringWithFormat:@"%@????", [NSString stringWithMultilingualWithKey:@"108755"]];
    
    _roleInfoViewOC.m_serverTxt.text = _tmp;
    
    _tmp = [NSString stringWithFormat:@"%@:????", [NSString stringWithMultilingualWithKey:@"102203"]];
    
    _roleInfoViewOC.m_levelTxt.text = _tmp;
}

#pragma mark -
#pragma mark Notify Action

-(void)notificationForUpdateOwerPlayerInfoWithNotify:(NSNotification*)vNotify{
    
    int total = GlobalData::shared()->playerInfo.buildingPower+GlobalData::shared()->playerInfo.sciencePower+GlobalData::shared()->playerInfo.armyPower+GlobalData::shared()->playerInfo.fortPower+GlobalData::shared()->playerInfo.questpower+GlobalData::shared()->playerInfo.playerPower+GlobalData::shared()->playerInfo.heroPower + GlobalData::shared()->playerInfo.equipPower;
    
     NSString* _tmp = [NSString stringWithFormat:@"%@:%@", [NSString stringWithMultilingualWithKey:@"102163"], [NSString stringWithUTF8String:CC_CMDITOA(total).c_str()]];
    _roleInfoViewOC.m_powerTxt.text = _tmp;
    
    _tmp = [NSString stringWithFormat:@"%@:%@", [NSString stringWithMultilingualWithKey:@"105001"], [NSString stringWithUTF8String:CC_CMDITOA(GlobalData::shared()->playerInfo.armyKill).c_str()]];
    _roleInfoViewOC.m_killTxt.text = _tmp;
    
    
    _tmp = [NSString stringWithFormat:@"%@:%@", [NSString stringWithMultilingualWithKey:@"102203"], [NSString stringWithFormat:@"%d", GlobalData::shared()->playerInfo.level]];
    _roleInfoViewOC.m_levelTxt.text = _tmp;
    
    [self initPlayerStats];
    
}

-(void)notificationForUpdatePlayerInfoWithNotify:(NSNotification*)vNotify{
    NSDictionary *userInfo =vNotify.userInfo;
    
    if(OCCppHelper::share()->m_info == NULL)
        return;
    _refleshStop = YES;
    if(_refleshView!=nil)
        _refleshView.hidden = YES;
    
    std::string leagueShort = "";
    if(OCCppHelper::share()->m_info->isInAlliance()){
        leagueShort = std::string("(") + OCCppHelper::share()->m_info->allianceInfo.shortName + ")";
    }
    
    std::string _pic = OCCppHelper::share()->m_info->getPic();
    
    if (OCCppHelper::share()->m_info->isUseCustomPic())
    {
        dispatch_async(self.dispatch_getQueue, ^{
            
            NSURL * url = [NSURL URLWithString:[NSString stringWithUTF8String:OCCppHelper::share()->m_info->getCustomPicUrl().c_str()]];
            NSData * data = [[NSData alloc]initWithContentsOfURL:url];
            UIImage *image = [[UIImage alloc]initWithData:data];
            
            if (data != nil) {
                dispatch_async(dispatch_get_main_queue(), ^{
                    [_roleInfoViewOC.m_headImg setImage: image];
                });
            }
        });
    }
    else if (!_pic.empty() && !OCCppHelper::share()->m_info->isUseCustomPic())
    {
        [_roleInfoViewOC.m_headImg setImage: [UIImage imageNamed:[NSString stringWithUTF8String:_pic.c_str()]]];
    }
    
    if(OCCppHelper::share()->m_info->uid == GlobalData::shared()->playerInfo.uid)
    {
        
        std::string total = CC_CMDITOA(OCCppHelper::share()->m_info->power);  //
        
        NSString* _tmp = [NSString stringWithFormat:@"%@%@", [NSString stringWithMultilingualWithKey:@"108755"], [NSString stringWithUTF8String:CC_ITOA(GlobalData::shared()->playerInfo.selfServerId)]];
        
        
        if (GlobalData::shared()->serverType==SERVER_BATTLE_FIELD) {
            
            _tmp = [NSString stringWithFormat:@"%@%@", [NSString stringWithMultilingualWithKey:@"108755"], [NSString stringWithUTF8String:CC_ITOA(GlobalData::shared()->playerInfo.crossFightSrcServerId)]];
        }

        _roleInfoViewOC.m_serverTxt.text = _tmp;
        
        string str = leagueShort;
        str.append(GlobalData::shared()->playerInfo.name);
        
        _tmp = [NSString stringWithFormat:@"%@", [NSString stringWithUTF8String:str.c_str()]];
        _roleInfoViewOC.m_nameTxt.text = _tmp;
        
        
        float achieve = AchievementController::getInstance()->getAchieveProgress();
        
        _tmp = [NSString stringWithFormat:@"%@%@%%", [NSString stringWithMultilingualWithKey:@"137409"], [NSString stringWithUTF8String:CCString::createWithFormat("%.f", achieve*100)->getCString()]];
        _roleInfoViewOC.m_achieveTTF.text = _tmp;
        
        _tmp = [NSString stringWithFormat:@"%@%@", [NSString stringWithMultilingualWithKey:@"137410"], [NSString stringWithUTF8String:CC_ITOA(AchievementController::getInstance()->getMedalComCnt())]];
        _roleInfoViewOC.m_medalTTF.text = _tmp;
        

    }
    else
    {
        
        std::string total = CC_CMDITOA(OCCppHelper::share()->m_info->power);  //
        
        NSString* _tmp = [NSString stringWithFormat:@"%@:%@", [NSString stringWithMultilingualWithKey:@"102163"], [NSString stringWithUTF8String:total.c_str()]];
        
        _roleInfoViewOC.m_powerTxt.text = _tmp;
        
        total = CC_CMDITOA(OCCppHelper::share()->m_info->armyKill);
        _tmp = [NSString stringWithFormat:@"%@:%@", [NSString stringWithMultilingualWithKey:@"105001"], [NSString stringWithUTF8String:total.c_str()]];
        
        _roleInfoViewOC.m_killTxt.text = _tmp;
        
         _tmp = [NSString stringWithFormat:@"%@%@%%", [NSString stringWithMultilingualWithKey:@"137409"], [NSString stringWithUTF8String:OCCppHelper::share()->m_achievePro.c_str()]];
        _roleInfoViewOC.m_achieveTTF.text = _tmp;
        
        _tmp = [NSString stringWithFormat:@"%@%@", [NSString stringWithMultilingualWithKey:@"137410"], [NSString stringWithUTF8String:OCCppHelper::share()->m_medalCntStr.c_str()]];
         _roleInfoViewOC.m_medalTTF.text = _tmp;
        
        string str = leagueShort;
        str.append(OCCppHelper::share()->m_info->name);
        
        _tmp = [NSString stringWithFormat:@"%@", [NSString stringWithUTF8String:str.c_str()]];
        _roleInfoViewOC.m_nameTxt.text = _tmp;
        
        _tmp = [NSString stringWithFormat:@"%@%@", [NSString stringWithMultilingualWithKey:@"108755"], [NSString stringWithUTF8String:CC_ITOA(OCCppHelper::share()->m_info->selfServerId)]];
        
        _roleInfoViewOC.m_serverTxt.text = _tmp;
        
        _tmp = [NSString stringWithFormat:@"%@:%@", [NSString stringWithMultilingualWithKey:@"102203"], [NSString stringWithFormat:@"%d", OCCppHelper::share()->m_info->level]];
        
        _roleInfoViewOC.m_levelTxt.text = _tmp;
        
        [self initPlayerStats];

    }

}

-(void)initPlayerStats
{

    int endNum=2;
    int area=2;
    if(OCCppHelper::share()->m_info->uid==GlobalData::shared()->playerInfo.uid){
        endNum=6;
        area=1;
    }
    float _hight = 0.f;
    for(;area<=endNum;area++){
        vector<int> attVec = GeneralManager::getInstance()->GeneralAttAreaMap[area];
        for (int i=0; i<attVec.size(); i++) {
            AttributeInfo& aInfo = GeneralManager::getInstance()->GeneralAttMap[attVec[i]];
            if (i==0) {
                RoleInfoItemTitleOC* _RoleInfoItemTitleOC = (RoleInfoItemTitleOC*)[self.scrollView viewWithTag:2*10+i];

                _RoleInfoItemTitleOC.m_title.text = [NSString stringWithMultilingualWithKey:[NSString stringWithUTF8String:aInfo.name.c_str()]];

                _hight += 29;
            }else{
                int value = OCCppHelper::share()->m_info->getAttributeByKey(aInfo.effect);
                if (value == -1) {
                    //                    value = CCCommonUtils::getEffectValueByNum(atoi(aInfo.effect.c_str()));
                    
                    value=0;
                    vector<std::string> vector;
                    CCCommonUtils::splitString(aInfo.effect, "|", vector);
                    //                    if(vector.size()==1){
                    //                        value = CCCommonUtils::getEffectValueByNum(atoi(vector[0].c_str()));
                    //                    }else if(vector.size()==2){
                    //                        value = CCCommonUtils::getEffectValueByNum(atoi(vector[1].c_str()));
                    //                        string temp = vector[0];
                    //                        vector.clear();
                    //                        CCCommonUtils::splitString(temp, ";", vector);
                    //                        if(vector.size()>0){
                    //                            value += CCCommonUtils::getEffectValueByNum(atoi(vector[0].c_str()));
                    //                        }
                    //                    }
                    for(int j=0;j<vector.size();j++){
                        value += CCCommonUtils::getEffectValueByNum(atoi(vector[j].c_str()));
                    }
                    //针对伤兵容量
                    if (aInfo.effect=="73|57") {
                        int sum = ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY);
                        //                        int sumOri = ArmyController::getInstance()->getMaxNumByType(TREAT_ARMY, true);
                        value = sum;// - sumOri;
                    }
                    //针对行军上限，出征士兵数量
                    if (aInfo.effect=="56") {
                        value = TroopsController::getInstance()->getMaxSoilder();
                    }
                    //针对行军部队总数
                    if (aInfo.effect=="55") {
                        value += 1;
                    }
                    //针对训练士兵数量
                    if (aInfo.effect=="131") {
                        int sum = ArmyController::getInstance()->getMaxNumByType(ARMY);
                        value =value+10+sum;
                    }
                    //城防值
                    if (aInfo.effect=="125") {
                        map<int, FunBuildInfo>::iterator it;
                        map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
                        for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
                        {
                            if ((it->second).type == FUN_BUILD_WALL) {
                                value += atoi( (it->second).para[2].c_str());
                                break;
                            }
                        }
                    }
                    //陷阱上限
                    if (aInfo.effect=="88") {
                        value = ArmyController::getInstance()->getMaxNumByType(FORT);
                    }
                    //训练士兵速度
                    if (aInfo.effect=="66") {
                        map<int, FunBuildInfo>::iterator it;
                        map<int, FunBuildInfo> *curBuildsInfo = FunBuildController::getInstance()->curBuildsInfo;
                        for(it=curBuildsInfo->begin(); it != curBuildsInfo->end(); ++it)
                        {
                            if ((it->second).type == FUN_BUILD_BARRACK) {
                                string resInfo = (it->second).information;
                                std::vector<std::string> lineItems;
                                CCCommonUtils::splitString(resInfo, "|", lineItems);
                                string lvLine = lineItems[(it->second).level];
                                std::vector<std::string> items;
                                CCCommonUtils::splitString(lvLine, ";", items);
                                if (items.size()==4) {
                                    string lvItem = items[2];
                                    std::vector<std::string> item;
                                    CCCommonUtils::splitString(lvItem, ",", item);
                                    if (item.size()==2) {
                                        value+=atoi(item[1].c_str());
                                    }
                                }
                            }
                        }
                    }
//
                }
                string valStr=CC_CMDITOA(value);
                if (aInfo.show==0) {//area>=3 && aInfo.aid!=46369 && aInfo.aid!=46370 && aInfo.aid!=46422 && aInfo.aid!=46410
                    valStr+="%";
                }
                if (aInfo.buff==1){
                    valStr="-"+valStr;
                }else if (aInfo.buff==2){
                    valStr="+"+valStr;
                }
                
                 RoleInfoItemOC* _RoleInfoItemOC = (RoleInfoItemOC*)[self.scrollView viewWithTag:2*10+i];
                
                _RoleInfoItemOC.m_nameTxt.text = [NSString stringWithMultilingualWithKey:[NSString stringWithUTF8String:aInfo.name.c_str()]];
                _RoleInfoItemOC.m_valueTxt.text = [NSString stringWithUTF8String:valStr.c_str()];
                
                int cell_type=i%2;

                _RoleInfoItemOC.m_bg0.hidden = cell_type==0?NO:YES;
                
                _hight += 23.5;
            }
        }
    }
    
//    self.scrollView.contentSize=CGSizeMake(self.infoListView.frame.size.width,_hight);
}


- (void)clickButton
{
    [self clearCache];
    [self dismissViewControllerAnimated:YES completion:nil];
}
-(void)rightButtonAction:(UIButton *)sender
{
    
}

// Override to allow orientations other than the default portrait orientation.
// This method is deprecated on ios6
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return UIInterfaceOrientationIsLandscape( interfaceOrientation );
}

// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
- (NSUInteger) supportedInterfaceOrientations{
#ifdef __IPHONE_6_0
    return UIInterfaceOrientationMaskPortrait;
#endif
}

- (BOOL) shouldAutorotate {
    return YES;
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden
{
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

-(void) clearCache
{
//    [[NSNotificationCenter defaultCenter] removeObserver:self name: IOS_kUPDATE_PLAYERINFO object:nil];
//    dispatch_release(self.dispatch_getQueue);
}


- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self name: IOS_kUPDATE_PLAYERINFO object:nil];
    [self.topHeaderView release];
    self.topHeaderView = nil;
    [self.headView release];
    self.headView = nil;
    [self.roleInfoViewOC release];
    self.roleInfoViewOC = nil;
    [self.infoListView release];
    [self.scrollView release];
    self.infoListView = nil;
    self.scrollView = nil;
    
    dispatch_release(self.dispatch_getQueue);
    
    [super dealloc];
}


@end
