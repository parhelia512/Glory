#pragma once
#include "StringTable.h"

namespace Glory
{
	class StringsOverrideTable : public StringTable
	{
	public:
		/** @brief Constructor */
		GLORY_LOCALIZE_API StringsOverrideTable();
		/** @override */
		GLORY_LOCALIZE_API StringsOverrideTable(UUID baseTableID, std::string&& language);
		/** @brief Destructor */
		GLORY_LOCALIZE_API virtual ~StringsOverrideTable();

		/** @brief Base table ID */
		GLORY_LOCALIZE_API UUID BaseTableID() const;
		/** @brief Set base table ID */
		GLORY_LOCALIZE_API void SetBaseTableID(UUID uuid);
		/** @brief Language */
		GLORY_LOCALIZE_API const std::string& Language() const;
		/** @brief Set language */
		GLORY_LOCALIZE_API void SetLanguage(std::string&& language);

	private:
		/** @brief Get a vector containing other resources referenced by this resource */
		virtual void References(IEngine*, std::vector<UUID>&) const override;

		virtual void Serialize(Utils::BinaryStream& container) const override;
		virtual void Deserialize(Utils::BinaryStream& container) override;

	private:
		UUID m_BaseTableID;
		std::string m_OverrideLanguage;
	};
}
