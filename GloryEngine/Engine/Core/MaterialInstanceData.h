#pragma once
#include "MaterialData.h"

namespace Glory
{
	class MaterialInstanceData : public MaterialData
	{
	public:
		MaterialInstanceData();
		MaterialInstanceData(UUID uuid, const std::string& name);
		MaterialInstanceData(MaterialData* pBaseMaterial);
		virtual ~MaterialInstanceData();

	public:
        virtual size_t ShaderCount() const override;
        virtual UUID GetShaderAt(size_t index) const override;

		MaterialData* GetBaseMaterial() const;
		void SetBaseMaterial(MaterialData* pMaterial);

		virtual UUID GetGPUUUID() const;

		void CopyOverrideStates(std::vector<bool>& destination);
		void PasteOverrideStates(std::vector<bool>& destination);

		virtual size_t PropertyInfoCount() const override;
		virtual MaterialPropertyInfo* GetPropertyInfoAt(size_t index) override;
		virtual size_t GetCurrentBufferOffset() const override;
		virtual std::vector<char>& GetBufferReference() override;
		virtual bool GetPropertyInfoIndex(const std::string& name, size_t& index) const override;
		virtual AssetReference<TextureData>* GetResourceUUIDPointer(size_t index) override;
		virtual size_t GetPropertyIndexFromResourceIndex(size_t index) const override;
		virtual size_t GetResourcePropertyCount() const override;
		virtual MaterialPropertyInfo* GetResourcePropertyInfo(size_t index) override;

		void ReloadProperties();

		bool IsPropertyOverriden(size_t index) const;
		virtual void EnableProperty(size_t index) override;

		void Serialize(BinaryStream& container) const override;
		void Deserialize(BinaryStream& container) override;

	private:
		virtual std::vector<char>& GetPropertyBuffer(size_t index) override;

	private:
		UUID m_BaseMaterialID;
		std::vector<bool> m_PropertyOverridesEnable;
	};
}
