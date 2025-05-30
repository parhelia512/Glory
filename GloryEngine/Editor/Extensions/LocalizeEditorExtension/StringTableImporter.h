#pragma once
#include <ImporterTemplate.h>
#include <StringsOverrideTable.h>
#include <Glory.h>

namespace Glory::Editor
{
    class StringTableImporter : public YAMLImporterTemplate<StringTable>
    {
	public:
		StringTableImporter() {};
		virtual ~StringTableImporter() {};

		std::string_view Name() const override;

	private:
		virtual bool SupportsExtension(const std::filesystem::path& extension) const override;
		virtual ImportedResource LoadResource(const std::filesystem::path& path, void*) const override;
		virtual bool SaveResource(const std::filesystem::path& path, StringTable* pStringTable) const override;

	private:
		virtual void Initialize() override;
		virtual void Cleanup() override {};
    };

	class StringsOverrideTableImporter : public YAMLImporterTemplate<StringsOverrideTable>
	{
	public:
		StringsOverrideTableImporter() {};
		virtual ~StringsOverrideTableImporter() {};

		std::string_view Name() const override;

	private:
		virtual bool SupportsExtension(const std::filesystem::path& extension) const override;
		virtual ImportedResource LoadResource(const std::filesystem::path& path, void*) const override;
		virtual bool SaveResource(const std::filesystem::path& path, StringsOverrideTable* pStringTable) const override;

	private:
		virtual void Initialize() override;
		virtual void Cleanup() override {};
	};
}
