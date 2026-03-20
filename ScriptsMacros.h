#pragma once
#define DECLARE_UPDATE_FLAG \
	static constexpr bool hasUpdate = true;
#define DECLARE_LATE_UPDATE_FLAG \
	static constexpr bool hasLateUpdate = true;
#define DECLARE_ON_CONSTRUCT_FLAG \
	static constexpr bool hasOnConstruct = true;
#define DECLARE_ON_ENABLE_FLAG \
	static constexpr bool hasOnEnable = true;
