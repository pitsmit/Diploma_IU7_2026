CXX = ccache g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wunused

INCLUDES = \
    -Isrc/ \
    -Isrc/commands \
    -Isrc/managers \
    -Isrc/essenses \
    -Isrc/facade \
    -Isrc/repositories \
    -Isrc/core \
    -Isrc/services \
    -Iexternal/spdlog/include \
    -Iapi-stubs/api \

SRC = $(shell find src/ api-stubs/api -name "*.cpp")
OBJ = $(SRC:.cpp=.o)
DEP = $(OBJ:.o=.d)

TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -lsqlite3 -lpistache -pthread -ludev -lblkid -lmount -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

-include $(DEP)

clean:
	rm -f $(OBJ) $(DEP) $(TARGET)

pre-bld-test:
	cmake -S . -B build

bld-test:
	cmake --build build

test:
	ctest --test-dir build --output-on-failure

dcr-bld-test:
	docker build -t usb-test .

dcr-test:
	mkdir -p coverage-report

	CID=$$(docker create \
		--privileged \
		-v /dev:/dev \
		usb-test) && \
	docker start -a $$CID && \
	docker cp $$CID:/app/coverage-report ./ && \
	docker rm $$CID

e2e:
	docker-compose up --build


install:
	@echo "Installing DLP USB policy..."

	@sudo mkdir -p /etc/udisks2
	@echo "[defaults]" | sudo tee /etc/udisks2/udisks2.conf > /dev/null
	@echo "automount=false" | sudo tee -a /etc/udisks2/udisks2.conf > /dev/null

	@printf '%s\n' \
'SUBSYSTEM=="block", ENV{UDISKS_AUTO}="0"' \
'SUBSYSTEM=="block", ENV{UDISKS_IGNORE}="1"' \
| sudo tee /etc/udev/rules.d/99-dlp-usb.rules > /dev/null

	@sudo systemctl restart udisks2
	@sudo udevadm control --reload-rules
	@sudo udevadm trigger

	@echo "Done. USB automount disabled for block devices."