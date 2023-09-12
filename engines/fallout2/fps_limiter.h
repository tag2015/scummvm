#ifndef FALLOUT2_FPS_LIMITER_H
#define FALLOUT2_FPS_LIMITER_H

namespace Fallout2 {

class FpsLimiter {
public:
	FpsLimiter(unsigned int fps = 60);
	void mark();
	void throttle() const;

private:
	const unsigned int _fps;
	unsigned int _ticks;
};

} // namespace Fallout2

#endif
