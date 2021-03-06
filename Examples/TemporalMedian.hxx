#pragma once
#include "../Include/Interface.hxx"

struct TemporalMedian final {
	static constexpr auto Name = "TemporalMedian";
	static constexpr auto Parameters = "clip:clip;radius:int:opt;";
	self(InputClip, Clip{});
	self(Radius, 1);
	auto Initialize(auto Arguments, auto Console) {
		InputClip = Arguments["clip"];
		if (Arguments["radius"].Exists())
			Radius = Arguments["radius"];
		if (!InputClip.WithConstantFormat() || !InputClip.WithConstantDimensions() || !InputClip.IsSinglePrecision())
			return Console.RaiseError("only single precision floating point clips with constant format and dimensions supported.");
		if (Radius < 0)
			return Console.RaiseError("radius cannot be negative!");
		return true;
	}
	auto RegisterVideoInfo(auto Core) {
		return InputClip.ExposeVideoInfo();
	}
	auto RequestReferenceFrames(auto Index, auto FrameContext) {
		InputClip.RequestFrames(Index, Radius, FrameContext);
	}
	auto DrawFrame(auto Index, auto Core, auto FrameContext) {
		auto InputFrames = InputClip.GetFrames<const float>(Index, Radius, PaddingPolicies::Temporal::Reflect, FrameContext);
		auto ProcessedFrame = Core.CreateNewFrameFrom(InputFrames[0]);
		auto Samples = std::vector<float>{};
		Samples.resize(2 * Radius + 1);
		for (auto c : Range{ ProcessedFrame.PlaneCount })
			for (auto y : Range{ ProcessedFrame[c].Height })
				for (auto x : Range{ ProcessedFrame[c].Width }) {
					for (auto t : Range{ -Radius, Radius + 1 })
						Samples[t + Radius] = InputFrames[t][c][y][x];
					std::nth_element(Samples.begin(), Samples.begin() + Radius, Samples.end());
					ProcessedFrame[c][y][x] = Samples[Radius];
				}
		return ProcessedFrame.Leak();
	}
};