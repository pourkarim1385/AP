CXX = g++
CXXFLAGS = -std=c++20

APPNAME = UTGame

CPPS = \
	main.cpp \
	DAL/Repository.cpp \
	Logic/RunProgram.cpp \
	models/CsvFile.cpp \
	models/Invitation.cpp \
	models/Match.cpp \
	models/Report.cpp \
	models/ServiceException.cpp \
	models/User.cpp \
	Service/CommandParser.cpp \
	Service/InviteService.cpp \
	Service/LoginService.cpp \
	Service/MatchMakingService.cpp \
	Service/ReportService.cpp \
	Service/RepositoryService.cpp \
	Service/MatchServices/MatchPlayService.cpp \
	Service/MatchServices/MatchService.cpp

OUTS = $(patsubst %.cpp,%.o,$(CPPS))

$(APPNAME): $(OUTS)
	$(CXX) $(CXXFLAGS) $(OUTS) -o $(APPNAME)

DAL/Repository.o: DAL/Repository.h
Logic/RunProgram.o: Logic/RunProgram.h

models/CsvFile.o: models/CsvFile.h
models/Invitation.o: models/Invitation.h
models/Match.o: models/Match.h
models/Report.o: models/Report.h
models/ServiceException.o: models/ServiceException.h
models/User.o: models/User.h

Service/CommandParser.o: Service/CommandParser.h
Service/InviteService.o: Service/InviteService.h
Service/LoginService.o: Service/LoginService.h
Service/MatchMakingService.o: Service/MatchMakingService.h
Service/ReportService.o: Service/ReportService.h
Service/RepositoryService.o: Service/RepositoryService.h
Service/MatchServices/MatchService.o: Service/MatchServices/MatchService.h
Service/MatchServices/MatchPlayService.o: Service/MatchServices/MatchPlayService.h


%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OUTS) $(APPNAME)
