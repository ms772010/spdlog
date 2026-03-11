#include <spdlog/business_logger.h>
#include <spdlog/spdlog.h>

int main() {
    spdlog::info("业务日志管理器示例程序启动");

    spdlog::BusinessLogger logger("./logs");

    logger.info(spdlog::BusinessType::ScreenRecord, "开始录制屏幕");
    logger.info(spdlog::BusinessType::ScreenRecord, "录制分辨率: 1920x1080");
    logger.info(spdlog::BusinessType::ScreenRecord, "录制帧率: 30fps");
    
    logger.info(spdlog::BusinessType::DesktopOpen, "打开桌面");
    logger.info(spdlog::BusinessType::DesktopOpen, "桌面窗口句柄: 0x12345678");
    logger.info(spdlog::BusinessType::DesktopOpen, "桌面坐标: (0, 0) - (1920, 1080)");

    logger.info(spdlog::BusinessType::KeyboardRecord, "开始录制键盘");
    logger.info(spdlog::BusinessType::KeyboardRecord, "监听键盘事件...");
    logger.warn(spdlog::BusinessType::KeyboardRecord, "检测到特殊按键按下");

    logger.info(spdlog::BusinessType::SoundRecord, "开始录制声音");
    logger.info(spdlog::BusinessType::SoundRecord, "音频采样率: 44100Hz");
    logger.info(spdlog::BusinessType::SoundRecord, "音频通道: 立体声");
    
    logger.error(spdlog::BusinessType::SoundRecord, "音频设备异常: {}", -1);
    
    logger.info(spdlog::BusinessType::ScreenRecord, "录制完成，文件大小: {}MB", 1024);
    logger.info(spdlog::BusinessType::KeyboardRecord, "录制完成，记录按键数: {}", 1234);
    logger.info(spdlog::BusinessType::SoundRecord, "录制完成，时长: {}秒", 3600);

    logger.flush_all();

    spdlog::info("业务日志示例完成");
    spdlog::info("生成的日志文件:");
    spdlog::info("  - logs/screen_record.log (最多3个,每个最大30MB)");
    spdlog::info("  - logs/desktop_open.log (最多3个,每个最大30MB)");
    spdlog::info("  - logs/keyboard_record.log (最多3个,每个最大30MB)");
    spdlog::info("  - logs/sound_record.log (最多3个,每个最大30MB)");

    return 0;
}
