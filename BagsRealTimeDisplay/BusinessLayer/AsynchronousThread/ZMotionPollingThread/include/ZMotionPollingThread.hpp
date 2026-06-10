#pragma once
#include <QThread>
#include <atomic>

// 前向声明，避免轮询线程头文件强依赖 ZMotionDevice
namespace rw { namespace hoep { class ZMotionDevice; } }

// Zmotion 实时状态（原子变量，跨线程共享）
struct ZMotionStatus
{
	std::atomic<double>   currentPosition{0.0};   // 当前轴位置 (DPOS)
	std::atomic<int32_t>  axisStatus{0};           // 轴状态
	std::atomic<bool>     isConnected{false};      // 连接状态
};

class ZMotionPollingThread : public QThread
{
	Q_OBJECT
public:
	explicit ZMotionPollingThread(rw::hoep::ZMotionDevice* device,
	                              ZMotionStatus* status,
	                              QObject* parent = nullptr);
	~ZMotionPollingThread() override;

	void startThread();

	void stopThread();
protected:
	void run() override;
signals:
	void zMotionStatusUpdated();
private:
	rw::hoep::ZMotionDevice* _device{nullptr};
	ZMotionStatus* _status{nullptr};
	std::atomic<bool> _running{false};
};
