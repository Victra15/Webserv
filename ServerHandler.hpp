#ifndef SERVER_HPP
# define SERVER_HPP
# define HEADER_END_SIZE 4

# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <sys/event.h>
# include <sys/types.h>
# include <sys/socket.h>
# include "HTTPRequest.hpp"
# include "HTTPResponse.hpp"
# include "webserv.hpp"

class ServerHandler {
	private:
		int							kq;
		int							listen_sock_fd;
		std::vector<struct kevent>	change_list;
		struct kevent				event_list[8];
		std::map<int, EventData*>	fd_list;
		// Conf						conf;

		void	changeEvent(const uintptr_t& ident,
							const int16_t& filter,
							const uint16_t& flags,
							const uint32_t& fflags,
							const intptr_t& data,
							void* const & udata);
		void	closeEvent(struct kevent * const & curr_event);

		// serverReady sub function
		int			serverListen(void);
		void		initListenerData(struct SocketData* listen_sock);

		// serverRun sub function
		void		keventError(const IdentType& event_id_type);
		void		handleListenEvent();
		void		handleClientEvent(struct kevent* const & curr_event);
		void		recvHeader(struct kevent* const & curr_event, SocketData* const & client_socket);
		void		recvBody(struct kevent* const & curr_event, SocketData* const & client_socket);
		void		sendResponse(struct kevent* const & curr_event, SocketData* const & client_socket);

	public:
		ServerHandler(void);
		virtual ~ServerHandler(void);

		void	serverReady(void);
		void	serverRun(void);
};

#endif
