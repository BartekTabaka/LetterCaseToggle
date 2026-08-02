#pragma once

namespace Core
{
	enum class Speed {
		Normal,
		Fast
	};

	struct TimingProfile {
		int settleWait;		// Short delay after an action (clear, focus, set text) before checking the result
		int copyTimeout;	// Max time to wait for the fast copy path (WM_COPY) to complete
		int ctrlCTimeout;	// Max time to wait for the simulated Ctrl+C operation to complete
		int pasteWait;		// Time to wait for Ctrl+V to finish before restoring the previous clipboard contents
	};

	const TimingProfile& GetTimingProfile(Speed speed);
}
