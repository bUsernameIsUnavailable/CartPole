// © 2023 Kaya Adrian.

#include "AtumExampleLibrary.h"

#include "FunctionLibraries/AtumLibraryTensors.h"
#include "Layers/Network/AtumNeuralNetwork.h"
#include "Optimizers/IAtumOptimizer.h"
#include "Tensors/AtumTensorDouble.h"


void UAtumExampleLibrary::PixelsToByteArray(const TArray<FColor>& In, TArray<uint8>& Out)
{
	const uint64&& ColourCount = In.Num();
	Out.SetNumUninitialized(ColourCount);
	
	for (uint64&& Index = 0ULL; Index < ColourCount; ++Index)
	{
		const FColor& Colour = In[Index];
		
		Out[Index] = Colour.R;
	}
}

bool UAtumExampleLibrary::SaveByteArrayToFile(const TArray<uint8>& Bytes, const FString& Path)
{
	return FFileHelper::SaveArrayToFile(Bytes, *Path);
}

UAtumTensorDouble* UAtumExampleLibrary::ImageToTensor(const TArray<FColor>& Pixels, const TArray<int64>& Sizes)
{
	auto* const Tensor = CastChecked<UAtumTensorDouble>(
		UAtumLibraryTensors::Empty(UAtumTensorDouble::StaticClass(), Sizes)
	);
	
	TArray<uint8> Bytes;
	PixelsToByteArray(Pixels, Bytes);
	
	Tensor->SetValues<double>(TArray<double>(Bytes).GetData(), Sizes);
	return Tensor;
}

double UAtumExampleLibrary::Train(
	UAtumNeuralNetwork* const Network,
	const TScriptInterface<IAtumOptimizer>& Optimiser,
	const int32 MaxMemory,
	const int32 BatchSize,
	const double Epsilon,
	const double EpsilonMin,
	const double EpsilonDecay,
	const double Gamma,
	const TArray<UAtumTensorDouble*>& StateMemory,
	const TArray<UAtumTensorDouble*>& NewStateMemory,
	const TArray<int32>& RewardMemory,
	const TArray<int32>& ActionMemory,
	const TArray<bool>& TerminalMemory,
	double& Loss,
	const EAtumTensorDeviceType DeviceType
)
{
	const c10::DeviceType Device = AtumEnums::Cast(DeviceType);
	if (MaxMemory < BatchSize)
		return Epsilon;
	
	TArray<double> StateBatch;
	TArray<double> NewStateBatch;
	const at::Tensor& RewardBatch = torch::zeros({ BatchSize }, c10::kInt).to(Device);
	const at::Tensor& ActionBatch = torch::zeros({ BatchSize }, c10::kInt).to(Device);
	const at::Tensor& TerminalBatch = torch::zeros({ BatchSize }, c10::kBool).to(Device);
	
	StateBatch.Reserve(BatchSize * 16384);
	NewStateBatch.Reserve(BatchSize * 16384);
	const at::Tensor& BatchIndices = torch::randint(MaxMemory, { BatchSize }, c10::kInt).to(Device);
	
	for (int32 Index = 0; Index < BatchSize; ++Index)
	{
		const int32 BatchIndex = BatchIndices[Index].item<int32>();
		StateBatch.Append(StateMemory[BatchIndex]->GetInternalValues());
		NewStateBatch.Append(NewStateMemory[BatchIndex]->GetInternalValues());
		RewardBatch[Index] = RewardMemory[BatchIndex];
		ActionBatch[Index] = ActionMemory[BatchIndex];
		TerminalBatch[Index] = TerminalMemory[BatchIndex];
	}
	
	TScriptInterface<IAtumTensor> QValues;
	{
		auto* const AtumStateBatch = NewObject<UAtumTensorDouble>();
		AtumStateBatch->SetValues<double>(StateBatch.GetData(), { BatchSize, 1LL, 128LL, 128LL });
		(*Network)(AtumStateBatch, QValues);
	}
	
	TScriptInterface<IAtumTensor> QNext;
	{
		auto* const AtumNewStateBatch = NewObject<UAtumTensorDouble>();
		AtumNewStateBatch->SetValues<double>(NewStateBatch.GetData(), { BatchSize, 1LL, 128LL, 128LL });
		(*Network)(AtumNewStateBatch, QNext);
	}
	
	const at::Tensor& InputValues = QValues->GetDataChecked().to(Device).index(
		{ torch::arange(BatchSize), ActionBatch }
	).to(Device);
	const at::Tensor& TargetValues = RewardBatch + ~TerminalBatch * Gamma * std::get<0>(
		QNext->GetDataChecked().to(Device).max(1LL)
	).to(Device);
	
	const at::Tensor& LossTensor = smooth_l1_loss(InputValues, TargetValues).to(Device);
	IAtumOptimizer::Execute_SetGradientToZero(Optimiser.GetObject(), true);
	LossTensor.backward();
	Optimiser->Step(UAtumTensorDouble::StaticClass());
	
	Loss = LossTensor.item<double>();
	return std::max(Epsilon * EpsilonDecay, EpsilonMin);
}

bool UAtumExampleLibrary::SaveEpisodeData(
	const int32 Episode,
	const int32 Reward,
	const double Epsilon,
	const double Loss,
	const FString& FilePath
)
{
	return FFileHelper::SaveStringToFile(
		FString::Printf(TEXT("\n%d,%d,%f,%f"), Episode, Reward, Epsilon, Loss),
		*FilePath,
		FFileHelper::EEncodingOptions::AutoDetect,
		&IFileManager::Get(),
		FILEWRITE_Append
	);
}
