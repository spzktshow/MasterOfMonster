LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp


LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/ApplicationMainScene.cpp \
                   ../../Classes/constellation/BehaviorTree.cpp \
                   ../../Classes/moonSugar/MSDataStructure.cpp \
                   ../../Classes/moonSugar/MSGeometry.cpp \
                   ../../Classes/moonSugar/MSMap.cpp \
                   ../../Classes/moonSugar/MSSource.cpp \
                   ../../Classes/moonSugar/MSVectorMap.cpp \
                   ../../Classes/moonSugar/MSVectorMapItem.cpp \
                   ../../Classes/moonSugar/TypeConver.cpp \
                   ../../Classes/moonSugar/VectorAlgorithm.cpp \
                   ../../Classes/LengendOfFightingDemons/actor/LOFDCampContext.cpp \
                   ../../Classes/LengendOfFightingDemons/loadScene/SmartLoadScene.cpp \
                   ../../Classes/LengendOfFightingDemons/event/SkillEvent.cpp \
                   ../../Classes/LengendOfFightingDemons/loadScene/LoadScene.cpp \
                   ../../Classes/LengendOfFightingDemons/scene/LOFDScene.cpp \
                   ../../Classes/LengendOfFightingDemons/config/LOFDConfigManager.cpp \
                   ../../Classes/LengendOfFightingDemons/actor/LOFDSkill.cpp \
                   ../../Classes/LengendOfFightingDemons/actor/LOFDActorAi.cpp \
                   ../../Classes/LengendOfFightingDemons/actor/LOFDActorBehavior.cpp \
                   ../../Classes/LengendOfFightingDemons/actor/LOFDActorState.cpp \
                   ../../Classes/LengendOfFightingDemons/actor/LOFDActor.cpp \
                   ../../Classes/LengendOfFightingDemons/actor/LOFDSkillData.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/moonSugar
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/LengendOfFightingDemons
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/LengendOfFightingDemons/event
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/LengendOfFightingDemons/loadScene
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/LengendOfFightingDemons/scene
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/LengendOfFightingDemons/fileFormat
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/LengendOfFightingDemons/config
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/LengendOfFightingDemons/actor
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/constellation

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dxandroid_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)
$(call import-module,Box2D)
$(call import-module,editor-support/cocostudio)
