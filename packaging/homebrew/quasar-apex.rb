class QuasarApex < Formula
  desc "High-performance portable C++ text search toolkit"
  homepage "https://github.com/OWNER/REPO"
  url "https://github.com/OWNER/REPO/archive/refs/tags/v10.0.0.tar.gz"
  sha256 "REPLACE_WITH_REAL_SHA256"
  license "Apache-2.0"

  depends_on "cmake" => :build

  def install
    system "cmake", "-S", ".", "-B", "build", "-DCMAKE_BUILD_TYPE=Release"
    system "cmake", "--build", "build", "-j"
    system "cmake", "--install", "build", "--prefix", prefix
    man1.install "man/quasar-grep.1"
    bash_completion.install "completions/bash/quasar-grep"
    zsh_completion.install "completions/zsh/_quasar-grep"
    fish_completion.install "completions/fish/quasar-grep.fish"
  end

  test do
    system "#{bin}/quasar-grep", "--help"
  end
end
