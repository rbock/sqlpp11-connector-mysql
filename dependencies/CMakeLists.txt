
include(FetchContent)

FetchContent_Declare(sqlpp11
    GIT_REPOSITORY  https://github.com/rbock/sqlpp11
    GIT_TAG         origin/master
)

FetchContent_MakeAvailable(sqlpp11)
