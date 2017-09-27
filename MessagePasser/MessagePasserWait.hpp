
template<typename StatusType>
void MessagePasser::WaitAll(std::vector<StatusType>& statuses) const {
    for (auto& status:statuses) {
        status.wait();
    }
}
