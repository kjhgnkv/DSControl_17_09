set(models
    manager/models/DataBaseModel.cpp
    manager/models/DataBaseModel.hpp
    manager/models/LocalFileSystemModel.cpp
    manager/models/LocalFileSystemModel.hpp
    )

set(manager
    manager/ConfigLoader.cpp
    manager/ConfigLoader.hpp
    manager/RepositoryManager.cpp
    manager/RepositoryManager.hpp
    )

set(repository
    repository/AbstractController.cpp
    repository/AbstractController.hpp
    repository/ComponentMimeData.cpp
    repository/ComponentMimeData.hpp
    repository/DataBaseController.cpp
    repository/DataBaseController.hpp
    repository/DataBaseQuery.hpp
    repository/DataBaseSettings.hpp
    repository/FileSystemController.cpp
    repository/FileSystemController.hpp
    )

set(ui
    ui/ChooserDialog.cpp
    ui/ChooserDialog.hpp
    ui/ChooserDialog.ui
    ui/Repository.cpp
    ui/Repository.hpp
    ui/Repository.ui
    )