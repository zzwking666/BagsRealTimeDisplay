#pragma once
#include <QThread>
#include <atomic>

class ZMotionPollingThread : public QThread
{
	Q_OBJECT
public:
	explicit ZMotionPollingThread(QObject* parent = nullptr);
	~ZMotionPollingThread() override;

	void startThread();

	void stopThread();
protected:
	void run() override;
signals:
	
private:
	std::atomic<bool> _running{false};
};
