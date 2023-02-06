template <typename T> using unique = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr unique<T> create_unique(Args &&...args) {
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T> using shared = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr shared<T> create_shared(Args &&...args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}
