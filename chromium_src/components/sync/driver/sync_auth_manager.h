#ifndef BRAVE_CHROMIUM_SRC_COMPONENTS_SYNC_DRIVER_SYNC_AUTH_MANAGER_H_
#define BRAVE_CHROMIUM_SRC_COMPONENTS_SYNC_DRIVER_SYNC_AUTH_MANAGER_H_
#include "brave/components/brave_sync/access_token_consumer.h"
#include "brave/components/brave_sync/access_token_fetcher_impl.h"
#include "services/network/public/cpp/shared_url_loader_factory.h"
#define BRAVE_SYNC_AUTH_MANAGER_H_                                          \
  void CreateAccessTokenFetcher(                                            \
      scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory);   \
  void DeriveSigningKeys(const std::string& seed);                          \
  void OnGetTokenSuccess(                                                   \
      const brave_sync::AccessTokenConsumer::TokenResponse& token_response) \
      override;                                                             \
  void OnGetTokenFailure(const std::string& error) override;                \
                                                                            \
 private:                                                                   \
  void GenerateClientIdAndSecret(std::string* client_id,                    \
                                 std::string* client_secrect,               \
                                 std::string* timestamp);                   \
  std::vector<uint8_t> public_key_;                                         \
  std::vector<uint8_t> private_key_;                                        \
  std::unique_ptr<brave_sync::AccessTokenFetcher> access_token_fetcher_;
#include "../../../../../components/sync/driver/sync_auth_manager.h"
#undef BRAVE_SYNC_AUTH_MANAGER_H_
#endif  // BRAVE_CHROMIUM_SRC_COMPONENTS_SYNC_DRIVER_SYNC_AUTH_MANAGER_H_
