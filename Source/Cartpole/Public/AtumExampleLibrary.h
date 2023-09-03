// © 2023 Kaya Adrian.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "AtumExampleLibrary.generated.h"

class IAtumOptimizer;
class UAtumNeuralNetwork;
class UAtumTensorDouble;
enum class EAtumTensorDeviceType : uint8;


UCLASS(Abstract)
class CARTPOLE_API UAtumExampleLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static void PixelsToByteArray(const TArray<FColor>& In, TArray<uint8>& Out);
	
	UFUNCTION(BlueprintCallable)
	static bool SaveByteArrayToFile(const TArray<uint8>& Bytes, const FString& Path);
	
	UFUNCTION(BlueprintCallable)
	static UAtumTensorDouble* ImageToTensor(const TArray<FColor>& Pixels, const TArray<int64>& Sizes);
	
	UFUNCTION(BlueprintCallable)
	static double Train(
		UAtumNeuralNetwork* Network,
		const TScriptInterface<IAtumOptimizer>& Optimiser,
		int32 MaxMemory,
		int32 BatchSize,
		double Epsilon,
		double EpsilonMin,
		double EpsilonDecay,
		double Gamma,
		const TArray<UAtumTensorDouble*>& StateMemory,
		const TArray<UAtumTensorDouble*>& NewStateMemory,
		const TArray<int32>& RewardMemory,
		const TArray<int32>& ActionMemory,
		const TArray<bool>& TerminalMemory,
		UPARAM(ref) double& Loss,
		EAtumTensorDeviceType DeviceType
	);
	
	UFUNCTION(BlueprintCallable)
	static bool SaveEpisodeData(int32 Episode, int32 Reward, double Epsilon, double Loss, const FString& FilePath);
};
